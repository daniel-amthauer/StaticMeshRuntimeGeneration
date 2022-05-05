#include "AssetTypeActions_StaticMeshRuntimeDescriptor.h"

#include "StaticMeshRuntimeDescriptor.h"

UClass* FAssetTypeActions_StaticMeshRuntimeDescriptor::GetSupportedClass() const
{
	return UStaticMeshRuntimeDescriptor::StaticClass();
}

FText FAssetTypeActions_StaticMeshRuntimeDescriptor::GetAssetDescription(const FAssetData& AssetData) const
{
	return FAssetTypeActions_Base::GetAssetDescription(AssetData);
}
