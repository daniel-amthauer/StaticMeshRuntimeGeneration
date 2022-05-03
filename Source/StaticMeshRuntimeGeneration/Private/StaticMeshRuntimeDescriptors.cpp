// Copyright Daniel Amthauer. All Rights Reserved


#include "StaticMeshRuntimeDescriptors.h"

void UStaticMeshRuntimeDescriptors::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);
	Ar << MeshDescriptions;
}
