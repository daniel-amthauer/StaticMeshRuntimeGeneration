﻿// Copyright Daniel Amthauer. All Rights Reserved


#include "StaticMeshRuntimeDescriptor.h"

#include "MeshAttributes.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshOperations.h"
#include "PhysicsEngine/BodySetup.h"


#if ENGINE_MAJOR_VERSION < 5
template <typename ElementIDType, typename AttributeType>
class TMeshAttributesRef_AccessHack : public TMeshAttributesRef<ElementIDType, AttributeType>
{
public:
	auto GetRawArray(int Index = 0)
	{
		auto ConstData = this->ArrayPtr->GetArrayForIndex(Index).GetData();
		auto Data = const_cast<AttributeType*>(ConstData);
		return MakeArrayView(Data, this->ArrayPtr->GetArrayForIndex(Index).Num());
	}
};
template <typename ElementIDType, typename AttributeType>
TMeshAttributesRef_AccessHack<ElementIDType, AttributeType>& MakeAccessHack(TMeshAttributesRef<ElementIDType, AttributeType>&& Attributes)
{
	return static_cast<TMeshAttributesRef_AccessHack<ElementIDType, AttributeType>&>(Attributes);
}

template <typename ElementIDType, typename AttributeType>
TMeshAttributesRef_AccessHack<ElementIDType, AttributeType>& MakeAccessHack(TMeshAttributesRef<ElementIDType, AttributeType>& Attributes)
{
	return static_cast<TMeshAttributesRef_AccessHack<ElementIDType, AttributeType>&>(Attributes);
}
#endif


void UStaticMeshRuntimeDescriptor::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
#if	0//WITH_EDITOR
	if (Ar.IsLoading())
	{
		RefreshDescriptors();
	}
	if (Ar.IsSaving())
#endif
	{
		Ar << MeshDescriptions;
	}
}

UStaticMesh* UStaticMeshRuntimeDescriptor::CreateRuntimeStaticMeshFromDescriptor(
	const UStaticMeshRuntimeDescriptor* Descriptor, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform,
	UObject* Outer)
{
	if (Descriptor)
	{
		//Copy the mesh descriptions and apply transformation to vertex positions
		const TArray<FMeshDescription> Descriptions = CreateTransformedMeshDescriptors(Descriptor, VertexTransform);
		TArray<FStaticMaterial> Empty;
		const auto StaticMesh = CreateRuntimeStaticMeshFromDescriptions(Descriptions, Descriptor->OriginalMesh ? Descriptor->OriginalMesh->GetStaticMaterials() : Empty, Outer);
		return StaticMesh;
	}
	return nullptr;
}

UStaticMesh* UStaticMeshRuntimeDescriptor::CreateRuntimeStaticMeshFromDescriptions(
	TArray<FMeshDescription> const& Descriptions, TArray<FStaticMaterial> const& StaticMaterials, UObject* Outer)
{
	//Create an array of pointers to the descriptions, which is the required input for BuildFromMeshDescriptions
	TArray<const FMeshDescription*> DescriptionPtrs;
	DescriptionPtrs.Reserve(Descriptions.Num());
	for (int i = 0; i < Descriptions.Num(); ++i)
	{
		DescriptionPtrs.Add(&Descriptions[i]);
	}

	//Create a new transient static mesh from the modified descriptions
	auto* StaticMesh = NewObject<UStaticMesh>(Outer, NAME_None, RF_Transient);
	UStaticMesh::FBuildMeshDescriptionsParams Params;
#if ENGINE_MAJOR_VERSION >= 5
	Params.bFastBuild = true;
#endif
	Params.bCommitMeshDescription = false;
	Params.bAllowCpuAccess = true;
	StaticMesh->bAllowCPUAccess = true;
	StaticMesh->SetStaticMaterials(StaticMaterials);
	bool bValid;
	{
		QUICK_SCOPE_CYCLE_COUNTER(BuildFromMeshDescriptions);
		bValid = StaticMesh->BuildFromMeshDescriptions(DescriptionPtrs, Params);
	}
	if (bValid)
	{
		
		StaticMesh->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
		StaticMesh->GetBodySetup()->CreatePhysicsMeshes();
		return StaticMesh;
	}
	return nullptr;
}

TArray<FMeshDescription> UStaticMeshRuntimeDescriptor::CreateTransformedMeshDescriptors(
	const UStaticMeshRuntimeDescriptor* Descriptor, TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform)
{
	if (Descriptor)
	{
		//Copy the mesh descriptions and apply transformation to vertex positions
		TArray<FMeshDescription> Descriptions = Descriptor->MeshDescriptions;
		for (auto& Desc : Descriptions)
		{
			FStaticMeshAttributes Attributes(Desc);
			auto VertexPositions = Desc.GetVertexPositions().GetRawArray(0);
			//To preserve original normals under the given transformation, 
			//we create a temporary array of positions for the original vertex positions projected along their normal

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
		return Descriptions;
	}
	return TArray<FMeshDescription>();
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
			//patch up imported material names using mesh section map for original mesh
			auto MaterialSlotNames = MeshDescriptions.Last().PolygonGroupAttributes().GetAttributesRef<FName>(
				MeshAttribute::PolygonGroup::ImportedMaterialSlotName);
			auto& SectionInfo = OriginalMesh->GetSectionInfoMap();
			for (int s = 0; s < SectionInfo.GetSectionNumber(i); ++s)
			{
				MaterialSlotNames[s] = OriginalMesh->GetStaticMaterials()[SectionInfo.Get(i,s).MaterialIndex].MaterialSlotName;
			}
		}
		(void)MarkPackageDirty();
	}	
}
#endif

