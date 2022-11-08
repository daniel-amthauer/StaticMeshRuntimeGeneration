// Copyright Daniel Amthauer. All Rights Reserved
#pragma once

#include "Runtime/Launch/Resources/Version.h"

#include "IAssetTypeActions.h"

class FAssetTypeActions_Proxy : public IAssetTypeActions
{
public:
	FAssetTypeActions_Proxy(TSharedPtr<IAssetTypeActions> const& InBaseActions)
		: BaseActions(InBaseActions)
	{}
	
	// IAssetTypeActions Implementation
	virtual FText GetName(void) const override
	{
		return BaseActions->GetName();
	}
	virtual UClass* GetSupportedClass() const override
	{
		return BaseActions->GetSupportedClass();
	}
	virtual FColor GetTypeColor() const override
	{
		return BaseActions->GetTypeColor();
	}
	virtual uint32 GetCategories() override
	{
		return BaseActions->GetCategories();
	}
	
	virtual FString GetObjectDisplayName(UObject* Object) const override
	{
		return BaseActions->GetObjectDisplayName(Object);
	}

	virtual bool HasActions( const TArray<UObject*>& InObjects ) const override
	{
		return BaseActions->HasActions(InObjects);
	}

	virtual void GetActions( const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder ) override
	{
		BaseActions->GetActions(InObjects, MenuBuilder);
	}

	virtual void GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) override
	{
		BaseActions->GetActions(InObjects, Section);
	}

	virtual void OpenAssetEditor( const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>() ) override
	{
		BaseActions->OpenAssetEditor(InObjects, EditWithinLevelEditor);
	}

	virtual bool AssetsActivatedOverride(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType) override
	{
		return BaseActions->AssetsActivatedOverride(InObjects, ActivationType);
	}

	virtual TArray<FAssetData> GetValidAssetsForPreviewOrEdit(TArrayView<const FAssetData> InAssetDatas, bool bIsPreview) override
	{
		return BaseActions->GetValidAssetsForPreviewOrEdit(InAssetDatas, bIsPreview);
	}

#if ENGINE_MAJOR_VERSION > 4
	virtual bool CanRename(const FAssetData& InAsset, FText* OutErrorMsg) const override
	{
		return BaseActions->CanRename(InAsset, OutErrorMsg);
	}

	virtual bool CanDuplicate(const FAssetData& InAsset, FText* OutErrorMsg) const override
	{
		return BaseActions->CanDuplicate(InAsset, OutErrorMsg);
	}
#endif

	virtual bool CanFilter() override
	{
		return BaseActions->CanFilter();
	}

	virtual bool CanLocalize() const override
	{
		return BaseActions->CanLocalize();
	}

	virtual bool CanMerge() const override
	{
		return BaseActions->CanMerge();
	}

	virtual void Merge(UObject* InObject) override
	{
		BaseActions->Merge(InObject);
	}

	virtual void Merge(UObject* BaseAsset, UObject* RemoteAsset, UObject* LocalAsset, const FOnMergeResolved& ResolutionCallback) override
	{
		BaseActions->Merge(BaseAsset, RemoteAsset, LocalAsset, ResolutionCallback);
	}

	virtual bool ShouldForceWorldCentric() override
	{
		return BaseActions->ShouldForceWorldCentric();
	}

	virtual void PerformAssetDiff(UObject* OldAsset, UObject* NewAsset, const struct FRevisionInfo& OldRevision, const struct FRevisionInfo& NewRevision) const override
	{
		BaseActions->PerformAssetDiff(OldAsset, NewAsset, OldRevision, NewRevision);
	}

	virtual class UThumbnailInfo* GetThumbnailInfo(UObject* Asset) const override
	{
		return BaseActions->GetThumbnailInfo(Asset);
	}

	virtual EThumbnailPrimType GetDefaultThumbnailPrimitiveType(UObject* Asset) const override
	{
		return BaseActions->GetDefaultThumbnailPrimitiveType(Asset);
	}

	virtual const TArray<FText>& GetSubMenus() const override
	{
		return BaseActions->GetSubMenus();
	}

	virtual TSharedPtr<class SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override
	{
		return BaseActions->GetThumbnailOverlay(AssetData);
	}

	virtual bool IsImportedAsset() const override
	{
		return BaseActions->IsImportedAsset();
	}

	virtual void GetResolvedSourceFilePaths(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFilePaths) const override
	{
		BaseActions->GetResolvedSourceFilePaths(TypeAssets, OutSourceFilePaths);
	}

	virtual void GetSourceFileLabels(const TArray<UObject*>& TypeAssets, TArray<FString>& OutSourceFileLabels) const override
	{
		BaseActions->GetSourceFileLabels(TypeAssets, OutSourceFileLabels);
	}

	virtual void BuildBackendFilter(FARFilter& InFilter) override
	{
		BaseActions->BuildBackendFilter(InFilter);
	}
	
	virtual FText GetDisplayNameFromAssetData(const FAssetData& AssetData) const override
	{
		return BaseActions->GetDisplayNameFromAssetData(AssetData);
	}

#if ENGINE_MAJOR_VERSION > 4
	virtual FName GetFilterName() const override
	{
		return BaseActions->GetFilterName();
	}
#else
	virtual void AssetsActivated(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType) override
	{
		return BaseActions->AssetsActivated(InObjects, ActivationType);
	}
#endif
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		return BaseActions->GetAssetDescription(AssetData);
	}
	virtual void SetSupported(bool bInSupported) override
	{
		BaseActions->SetSupported(bInSupported);
	}
	virtual bool IsSupported() const override
	{
		return BaseActions->IsSupported();
	}

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0)
	virtual FTopLevelAssetPath GetClassPathName() const override
	{
		return BaseActions->GetClassPathName();
	}
#endif

private:
	TSharedPtr<IAssetTypeActions> BaseActions;
};
