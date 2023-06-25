// Copyright Daniel Amthauer. All Rights Reserved

#include "AssetToolsModule.h"
#include "ContentBrowserMenuContexts.h"
#include "PackageTools.h"
#include "StaticMeshRuntimeDescriptorFactory.h"
#include "Engine/StaticMesh.h"
#include "ToolMenuSection.h"


namespace MenuExtension_StaticMesh
{
	static void CreateSMRDs(const FToolMenuContext& InContext)
	{
		const UContentBrowserAssetContextMenuContext* Context = UContentBrowserAssetContextMenuContext::FindContextWithAssets(InContext);

		FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
		auto SMRDFactory = NewObject<UStaticMeshRuntimeDescriptorFactory>();
		TArray<UObject*> ObjectsToSync;
		
		for (UStaticMesh* StaticMesh : Context->LoadSelectedObjects<UStaticMesh>())
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
		GEditor->SyncBrowserToObjects(ObjectsToSync);
	}
  
	static FDelayedAutoRegisterHelper DelayedAutoRegister(EDelayedRegisterRunPhase::EndOfEngineInit, []{
	   UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
	   {
		  FToolMenuOwnerScoped OwnerScoped(UE_MODULE_NAME);
		  UToolMenu* Menu = UE::ContentBrowser::ExtendToolMenu_AssetContextMenu(UStaticMesh::StaticClass());
     
		  FToolMenuSection& Section = Menu->FindOrAddSection("GetAssetActions");
		  Section.AddDynamicEntry(NAME_None, FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
		  {
			  const TAttribute<FText> Label = INVTEXT("Create Static Mesh Runtime Descriptors");
			  const TAttribute<FText> ToolTip = INVTEXT("Create Static Mesh Runtime Descriptors from original meshes");
			  const FSlateIcon Icon = FSlateIcon();
  
 
			  FToolUIAction UIAction;
			  UIAction.ExecuteAction = FToolMenuExecuteAction::CreateStatic(&CreateSMRDs);
			  InSection.AddMenuEntry("StaticMesh_CreateSMRDs", Label, ToolTip, Icon, UIAction);
		  }));
	   }));
	});
}
