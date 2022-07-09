// Copyright Daniel Amthauer. All Rights Reserved
#pragma once
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_StaticMeshRuntimeDescriptor : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_StaticMeshRuntimeDescriptor(EAssetTypeCategories::Type InAssetCategory)
		: FAssetTypeActions_Base()
		, MyCategory(InAssetCategory)
	{}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return INVTEXT("Static Mesh Runtime Descriptor"); }
	virtual FColor GetTypeColor() const override { return FColor( 0, 194, 76 ); }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return MyCategory; }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	virtual void GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) override;
	virtual UThumbnailInfo* GetThumbnailInfo(UObject* Asset) const override;

private:
	EAssetTypeCategories::Type MyCategory;
};