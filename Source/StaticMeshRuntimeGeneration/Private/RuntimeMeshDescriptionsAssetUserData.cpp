// Copyright Daniel Amthauer. All Rights Reserved


#include "RuntimeMeshDescriptionsAssetUserData.h"

#include "MeshAttributes.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshOperations.h"
#include "PhysicsEngine/BodySetup.h"

void URuntimeMeshDescriptionsAssetUserData::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
	//If you ever have crashes when loading because of this
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

URuntimeMeshDescriptionsAssetUserData* URuntimeMeshDescriptionsAssetUserData::GetFrom(const UStaticMesh* StaticMesh)
{
	if (!StaticMesh)
		return nullptr;
	URuntimeMeshDescriptionsAssetUserData* UserData = CastChecked<URuntimeMeshDescriptionsAssetUserData>(
		const_cast<UStaticMesh*>(StaticMesh)->GetAssetUserDataOfClass(StaticClass()),
		ECastCheckedType::NullAllowed);
	if (!UserData)
	{
		FFrame::KismetExecutionMessage(TEXT("Static mesh did not have the required RuntimeMeshDescriptions Asset User Data"), ELogVerbosity::Warning);
		return nullptr;
	}
	return UserData;
}

UStaticMesh* URuntimeMeshDescriptionsAssetUserData::CreateTransformedStaticMesh(const UStaticMesh* OriginalMesh,
																				TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform, UObject* Outer)
{
	auto UserData = GetFrom(OriginalMesh);
	if (!UserData)
		return nullptr;
	//Copy the mesh descriptions and apply transformation to vertex positions
	const TArray<FMeshDescription> Descriptions = CreateTransformedMeshDescriptions(UserData, VertexTransform);
	TArray<FStaticMaterial> Empty;
	const auto StaticMesh = CreateRuntimeStaticMeshFromDescriptions(Descriptions, OriginalMesh->GetStaticMaterials(), Outer);
	return StaticMesh;
}

UStaticMesh* URuntimeMeshDescriptionsAssetUserData::CreateRuntimeStaticMeshFromDescriptions(
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
	Params.bFastBuild = true;
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
		
		StaticMesh->GetBodySetup()->CollisionTraceFlag = CTF_UseComplexAsSimple;
		StaticMesh->GetBodySetup()->CreatePhysicsMeshes();
		return StaticMesh;
	}
	return nullptr;
}

TArray<FMeshDescription> URuntimeMeshDescriptionsAssetUserData::CreateTransformedMeshDescriptions(
	const URuntimeMeshDescriptionsAssetUserData* DescriptionsUserData,
	TFunctionRef<void(TArrayView<FVector3f>)> VertexTransform)
{
	if (DescriptionsUserData)
	{
		//Copy the mesh descriptions and apply transformation to vertex positions
		TArray<FMeshDescription> Descriptions = DescriptionsUserData->MeshDescriptions;
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
void URuntimeMeshDescriptionsAssetUserData::PostEditChangeOwner()
{
	Super::PostEditChangeOwner();
	auto OriginalMesh = GetOuterUStaticMesh();
	if (!OriginalMesh)
		return;
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
#endif

