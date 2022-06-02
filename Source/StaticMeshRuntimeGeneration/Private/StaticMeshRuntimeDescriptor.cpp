// Copyright Daniel Amthauer. All Rights Reserved


#include "StaticMeshRuntimeDescriptor.h"

#include "MeshAttributes.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshOperations.h"

void UStaticMeshRuntimeDescriptor::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
	Ar << MeshDescriptions;
}

UStaticMesh* UStaticMeshRuntimeDescriptor::CreateRuntimeStaticMeshFromDescriptor(UStaticMeshRuntimeDescriptor* Descriptor, TFunction<void(TArrayView<FVector3f>)> VertexTransform)
{
	if (Descriptor)
	{
		//Copy the mesh descriptions and apply transformation to vertex positions
		TArray<FMeshDescription> Descriptions = Descriptor->MeshDescriptions;
		for (auto& Desc : Descriptions)
		{
			auto VertexPositions = Desc.GetVertexPositions().GetRawArray(0);
			//To preserve original normals under the given transformation, 
			//we create a temporary array of positions for the original vertex positions projected along their normal
			FStaticMeshAttributes Attributes(Desc);
			auto Normals = Attributes.GetVertexInstanceNormals().GetRawArray();
			TArray<FVector3f> TempNormalRefs;
			TempNormalRefs.Reserve(Normals.Num());
			auto VertexInstanceVertexIndices = Attributes.GetVertexInstanceVertexIndices().GetRawArray();
			check(Normals.Num() == VertexInstanceVertexIndices.Num());
			for (int vi = 0; vi < VertexInstanceVertexIndices.Num(); ++vi)
			{
				TempNormalRefs.Add(VertexPositions[VertexInstanceVertexIndices[vi]] - Normals[vi]);
			}
			//Now apply transform to these temp normal reference points
			VertexTransform(TempNormalRefs);

			//Now transform the vertex positions
			VertexTransform(VertexPositions);

			//After original verts have already been transformed, recompute normals using transformed normal ref points and
			//transformed vertex positions
			for (int vi = 0; vi < VertexInstanceVertexIndices.Num(); ++vi)
			{
				Normals[vi] = (VertexPositions[VertexInstanceVertexIndices[vi]] - TempNormalRefs[vi]).GetSafeNormal(); 
			}
			FStaticMeshOperations::ComputeMikktTangents(Desc, true);
		}

		//Create an array of pointers to the descriptions, which is the required input for BuildFromMeshDescriptions
		TArray<const FMeshDescription*> DescriptionPtrs;
		DescriptionPtrs.Reserve(Descriptions.Num());
		for (int i = 0; i < Descriptions.Num(); ++i)
		{
			DescriptionPtrs.Add(&Descriptions[i]);
		}

		//Create a new transient static mesh from the modified descriptions
		auto* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), NAME_None, RF_Transient);
		UStaticMesh::FBuildMeshDescriptionsParams Params;
		Params.bFastBuild = true;
		Params.bCommitMeshDescription = false;
		if (StaticMesh->BuildFromMeshDescriptions(DescriptionPtrs, Params))
		{
			//TODO: maybe cache the static materials in the asset as well, instead of requiring the original mesh for this
			if (Descriptor->OriginalMesh)
			{
				StaticMesh->SetStaticMaterials(Descriptor->OriginalMesh->GetStaticMaterials());
			}
			return StaticMesh;
		}
	}
	return nullptr;
}
UStaticMesh* UStaticMeshRuntimeDescriptor::K2_CreateRuntimeStaticMeshFromDescriptor(
	UStaticMeshRuntimeDescriptor* Descriptor, FBP_VertexTransform VertexTransform)
{
	return CreateRuntimeStaticMeshFromDescriptor(Descriptor, [=](TArrayView<FVector3f> OriginalVerts)
	{
		TArray<FVector> BP_Verts;
		BP_Verts.Reserve(OriginalVerts.Num());
		for (auto& Vert : OriginalVerts)
		{
			BP_Verts.Add(FVector(Vert));
		}
		BP_Verts = VertexTransform.Execute(BP_Verts);
		const int NumVertsToModify = FMath::Min(BP_Verts.Num(), OriginalVerts.Num());
		for (int v = 0; v < NumVertsToModify; ++v)
		{
			OriginalVerts[v] = FVector3f(BP_Verts[v]);
		}
	});
}

#if WITH_EDITOR
void UStaticMeshRuntimeDescriptor::RefreshDescriptors()
{
	if (OriginalMesh)
	{
		MeshDescriptions.Reset(OriginalMesh->GetNumSourceModels());
		for (int i = 0; i < OriginalMesh->GetNumSourceModels(); ++i)
		{
			MeshDescriptions.Add(*OriginalMesh->GetMeshDescription(i));
		}
	}	
}
#endif

