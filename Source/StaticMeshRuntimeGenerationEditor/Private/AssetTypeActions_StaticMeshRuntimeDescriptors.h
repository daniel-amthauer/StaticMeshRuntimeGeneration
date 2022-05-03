#pragma once
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_StaticMeshRuntimeDescriptors : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_StaticMeshRuntimeDescriptors(EAssetTypeCategories::Type InAssetCategory)
		: FAssetTypeActions_Base()
		, MyCategory(InAssetCategory)
	{}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return INVTEXT("Static Mesh Runtime Descriptors"); }
	virtual FColor GetTypeColor() const override { return FColor( 0, 194, 76 ); }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return MyCategory; }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;

private:
	EAssetTypeCategories::Type MyCategory;
};
