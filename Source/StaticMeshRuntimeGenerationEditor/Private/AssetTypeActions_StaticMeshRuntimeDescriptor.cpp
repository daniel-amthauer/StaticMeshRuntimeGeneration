// Copyright Daniel Amthauer. All Rights Reserved
#include "AssetTypeActions_StaticMeshRuntimeDescriptor.h"

#include "StaticMeshRuntimeDescriptor.h"
#include "StaticMeshRuntimeGenerationEditor.h"
#include "ToolMenuSection.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"

UClass* FAssetTypeActions_StaticMeshRuntimeDescriptor::GetSupportedClass() const
{
	return UStaticMeshRuntimeDescriptor::StaticClass();
}

FText FAssetTypeActions_StaticMeshRuntimeDescriptor::GetAssetDescription(const FAssetData& AssetData) const
{
	return FAssetTypeActions_Base::GetAssetDescription(AssetData);
}

bool FAssetTypeActions_StaticMeshRuntimeDescriptor::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FAssetTypeActions_StaticMeshRuntimeDescriptor::GetActions(const TArray<UObject*>& InObjects,
	FToolMenuSection& Section)
{
	const TArray<TWeakObjectPtr<UObject>> WeakObjects = GetTypedWeakObjectPtrs<UObject>(InObjects);

	Section.AddMenuEntry(
		"Refresh_Descriptors",
		INVTEXT("Refresh descriptors"),
		INVTEXT("Refreshes descriptors from original meshes"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]
			{
				for (auto& WeakObj : WeakObjects)
				{
					if (auto SMRD = Cast<UStaticMeshRuntimeDescriptor>(WeakObj.Get()))
					{
						SMRD->RefreshDescriptors();
					}
				}
			}),
			FCanExecuteAction())
		);

	FStaticMeshRuntimeGenerationEditor& Module = FStaticMeshRuntimeGenerationEditor::GetModule();
	if (Module.GetExtraActionsDelegate().IsBound())
	{
		Module.GetExtraActionsDelegate().Broadcast(WeakObjects, Section);
	}
}

UThumbnailInfo* FAssetTypeActions_StaticMeshRuntimeDescriptor::GetThumbnailInfo(UObject* Asset) const
{
	const auto* Mesh = CastChecked<UStaticMeshRuntimeDescriptor>(Asset)->OriginalMesh;
	UThumbnailInfo* ThumbnailInfo = Mesh->ThumbnailInfo;
	return ThumbnailInfo;
}
