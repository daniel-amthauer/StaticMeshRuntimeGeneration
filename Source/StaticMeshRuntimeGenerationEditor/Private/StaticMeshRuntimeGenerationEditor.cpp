#include "StaticMeshRuntimeGenerationEditor.h"

DEFINE_LOG_CATEGORY(StaticMeshRuntimeGenerationEditor);

#define LOCTEXT_NAMESPACE "FStaticMeshRuntimeGenerationEditor"

void FStaticMeshRuntimeGenerationEditor::StartupModule()
{
	UE_LOG(StaticMeshRuntimeGenerationEditor, Warning, TEXT("StaticMeshRuntimeGenerationEditor module has been loaded"));
}

void FStaticMeshRuntimeGenerationEditor::ShutdownModule()
{
	UE_LOG(StaticMeshRuntimeGenerationEditor, Warning, TEXT("StaticMeshRuntimeGenerationEditor module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStaticMeshRuntimeGenerationEditor, StaticMeshRuntimeGenerationEditor)