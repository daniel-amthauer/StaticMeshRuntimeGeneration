#include "StaticMeshRuntimeGenerationEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_StaticMeshRuntimeDescriptors.h"

DEFINE_LOG_CATEGORY(StaticMeshRuntimeGenerationEditor);

#define LOCTEXT_NAMESPACE "FStaticMeshRuntimeGenerationEditor"

void FStaticMeshRuntimeGenerationEditor::StartupModule()
{
	// Register the editor asset type actions.
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.RegisterAssetTypeActions(MakeShared<FAssetTypeActions_StaticMeshRuntimeDescriptors>(EAssetTypeCategories::Misc));
}

void FStaticMeshRuntimeGenerationEditor::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStaticMeshRuntimeGenerationEditor, StaticMeshRuntimeGenerationEditor)