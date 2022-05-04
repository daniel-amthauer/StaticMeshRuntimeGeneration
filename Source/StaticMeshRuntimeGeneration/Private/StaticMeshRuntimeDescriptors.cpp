// Copyright Daniel Amthauer. All Rights Reserved


#include "StaticMeshRuntimeDescriptors.h"

void UStaticMeshRuntimeDescriptors::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
	Ar << MeshDescriptions;
}

UStaticMesh* UStaticMeshRuntimeDescriptors::CreateRuntimeStaticMeshFromDescriptors(
	UStaticMeshRuntimeDescriptors* Descriptors)
{
	if (Descriptors)
	{
		auto* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), NAME_None, RF_Transient);
		UStaticMesh::FBuildMeshDescriptionsParams Params;
		Params.bFastBuild = true;
		Params.bCommitMeshDescription = false;
		TArray<const FMeshDescription*> DescriptionPtrs;
		DescriptionPtrs.Reserve(Descriptors->MeshDescriptions.Num());
		for (int i = 0; i < Descriptors->MeshDescriptions.Num(); ++i)
		{
			DescriptionPtrs.Add(&Descriptors->MeshDescriptions[i]);
		}
		if (StaticMesh->BuildFromMeshDescriptions(DescriptionPtrs, Params))
		{
			StaticMesh->SetStaticMaterials(Descriptors->OriginalMesh->GetStaticMaterials());
			return StaticMesh;
		}
	}
	return nullptr;
}

#if WITH_EDITOR
void UStaticMeshRuntimeDescriptors::RefreshDescriptors()
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

