// Copyright Daniel Amthauer. All Rights Reserved
#include "AssetTypeActions_StaticMesh_Custom.h"

#include "ObjectEditorUtils.h"
#include "PackageTools.h"
#include "StaticMeshRuntimeDescriptorFactory.h"
#include "Engine/StaticMesh.h"
#include "ToolMenuSection.h"

void FAssetTypeActions_StaticMesh_Custom::GetActions(const TArray<UObject*>& InObjects,
	FToolMenuSection& Section)
{
	FAssetTypeActions_Proxy::GetActions(InObjects, Section);
	
	const TArray<TWeakObjectPtr<UObject>> WeakObjects = FObjectEditorUtils::GetTypedWeakObjectPtrs<UObject>(InObjects);

	Section.AddMenuEntry(
		"Create Static Mesh Runtime Descriptors",
		INVTEXT("Create Static Mesh Runtime Descriptors"),
		INVTEXT("Create Static Mesh Runtime Descriptors from original meshes"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]
			{
				FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
				auto SMRDFactory = NewObject<UStaticMeshRuntimeDescriptorFactory>();
				TArray<UObject*> ObjectsToSync;
				for (auto& WeakObj : WeakObjects)
				{
					if (auto StaticMesh = Cast<UStaticMesh>(WeakObj.Get()))
					{
						// Determine an appropriate name
						const FString SanitizedBasePackageName = UPackageTools::SanitizePackageName(StaticMesh->GetOutermost()->GetName());
						const FString PackagePath = FPackageName::GetLongPackagePath(SanitizedBasePackageName);
						const FString BaseAssetNameWithPrefix = TEXT("SMRD_") + FPackageName::GetLongPackageAssetName(SanitizedBasePackageName);
						FString Name;
						FString PackageName;
						AssetToolsModule.Get().CreateUniqueAssetName(PackagePath / BaseAssetNameWithPrefix, TEXT(""), PackageName, Name);
						SMRDFactory->OriginalMesh = StaticMesh;
						UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, SMRDFactory->GetSupportedClass(), SMRDFactory);
						ObjectsToSync.Add(NewAsset);
					}
				}
				GEditor->SyncBrowserToObjects(ObjectsToSync);
			}),
			FCanExecuteAction())
		);
}
