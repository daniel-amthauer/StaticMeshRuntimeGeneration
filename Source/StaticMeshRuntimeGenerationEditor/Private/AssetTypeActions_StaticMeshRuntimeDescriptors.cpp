#include "AssetTypeActions_StaticMeshRuntimeDescriptors.h"

#include "StaticMeshRuntimeDescriptors.h"

UClass* FAssetTypeActions_StaticMeshRuntimeDescriptors::GetSupportedClass() const
{
	return UStaticMeshRuntimeDescriptors::StaticClass();
}

FText FAssetTypeActions_StaticMeshRuntimeDescriptors::GetAssetDescription(const FAssetData& AssetData) const
{
	return FAssetTypeActions_Base::GetAssetDescription(AssetData);
}
