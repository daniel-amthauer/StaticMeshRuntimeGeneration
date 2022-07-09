// Copyright Daniel Amthauer. All Rights Reserved
#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(StaticMeshRuntimeGenerationEditor, All, All);

class FStaticMeshRuntimeGenerationEditor : public IModuleInterface
{
	public:

	static inline FStaticMeshRuntimeGenerationEditor& GetModule()
	{
		static const FName ModuleName = "StaticMeshRuntimeGenerationEditor";
		return FModuleManager::LoadModuleChecked< FStaticMeshRuntimeGenerationEditor >(ModuleName);
	}

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FExtraActions, const TArray<TWeakObjectPtr<UObject>>& , FToolMenuSection& );
	FExtraActions& GetExtraActionsDelegate() { return ExtraActions; }
private:
	FExtraActions ExtraActions;
};