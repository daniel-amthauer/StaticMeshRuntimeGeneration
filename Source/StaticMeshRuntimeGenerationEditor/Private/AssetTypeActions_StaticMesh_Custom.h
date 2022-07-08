#pragma once
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_StaticMesh_Custom : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_StaticMesh_Custom(TSharedPtr<IAssetTypeActions> InEngineActions)
		: FAssetTypeActions_Base()
		, EngineActions(InEngineActions)
	{}
	
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return EngineActions->GetName(); }
	virtual FColor GetTypeColor() const override { return EngineActions->GetTypeColor(); }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return EngineActions->GetCategories(); }
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	virtual void GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) override;

private:
	TSharedPtr<IAssetTypeActions> EngineActions;
};
