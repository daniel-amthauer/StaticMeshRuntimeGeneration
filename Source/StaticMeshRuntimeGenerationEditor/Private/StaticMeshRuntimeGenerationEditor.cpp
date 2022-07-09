// Copyright Daniel Amthauer. All Rights Reserved
#include "StaticMeshRuntimeGenerationEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_StaticMeshRuntimeDescriptor.h"
#include "AssetTypeActions_StaticMesh_Custom.h"
#include "StaticMeshRuntimeDescriptor.h"
#include "StaticMeshRuntimeDescriptorThumbnailRenderer.h"
#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"

DEFINE_LOG_CATEGORY(StaticMeshRuntimeGenerationEditor);

#define LOCTEXT_NAMESPACE "FStaticMeshRuntimeGenerationEditor"

void FStaticMeshRuntimeGenerationEditor::StartupModule()
{
	// Register the editor asset type actions.
	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	AssetTools.RegisterAssetTypeActions(MakeShared<FAssetTypeActions_StaticMeshRuntimeDescriptor>(EAssetTypeCategories::Misc));

	TSharedPtr<IAssetTypeActions> EngineActions = AssetTools.GetAssetTypeActionsForClass(UStaticMesh::StaticClass()).Pin();
	AssetTools.RegisterAssetTypeActions(MakeShared<FAssetTypeActions_StaticMesh_Custom>(EngineActions));

	UThumbnailManager::Get().RegisterCustomRenderer(UStaticMeshRuntimeDescriptor::StaticClass(), UStaticMeshRuntimeDescriptorThumbnailRenderer::StaticClass());
}

void FStaticMeshRuntimeGenerationEditor::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStaticMeshRuntimeGenerationEditor, StaticMeshRuntimeGenerationEditor)