#include "AssetTypeActions_StaticMeshRuntimeDescriptor.h"

#include "StaticMeshRuntimeDescriptor.h"
#include "ToolMenuSection.h"

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
	for (auto Object : InObjects)
	{
		if (!Object->IsA<UStaticMeshRuntimeDescriptor>())
			return false;
	}
	return InObjects.Num() > 0;
}

void FAssetTypeActions_StaticMeshRuntimeDescriptor::GetActions(const TArray<UObject*>& InObjects,
	FToolMenuSection& Section)
{
	const TArray<TWeakObjectPtr<UObject>> SMRDs = GetTypedWeakObjectPtrs<UObject>(InObjects);

	Section.AddMenuEntry(
		"Refresh_Descriptors",
		INVTEXT("Refresh descriptors"),
		INVTEXT("Refreshes descriptors from original meshes"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]
			{
				for (auto& SMRDWeak : SMRDs)
				{
					if (auto SMRD = Cast<UStaticMeshRuntimeDescriptor>(SMRDWeak.Get()))
					{
						SMRD->RefreshDescriptors();
					}
				}
			}),
			FCanExecuteAction())
		);
}
