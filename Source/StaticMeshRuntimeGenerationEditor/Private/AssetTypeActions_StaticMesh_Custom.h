// Copyright Daniel Amthauer. All Rights Reserved
#pragma once
#include "AssetTypeActions_Base.h"
#include "AssetTypeActions_Proxy.h"

class FAssetTypeActions_StaticMesh_Custom : public FAssetTypeActions_Proxy
{
public:
	FAssetTypeActions_StaticMesh_Custom(TSharedPtr<IAssetTypeActions> InBaseActions)
		: FAssetTypeActions_Proxy(InBaseActions)
	{}
	
	// IAssetTypeActions Implementation
	virtual void GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) override;
};
