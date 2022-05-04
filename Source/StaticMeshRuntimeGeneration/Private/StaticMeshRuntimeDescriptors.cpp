// Copyright Daniel Amthauer. All Rights Reserved


#include "StaticMeshRuntimeDescriptors.h"

void UStaticMeshRuntimeDescriptors::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
	Ar << MeshDescriptions;
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

