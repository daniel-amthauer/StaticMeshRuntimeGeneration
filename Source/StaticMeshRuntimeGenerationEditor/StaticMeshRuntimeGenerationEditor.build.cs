// Copyright Daniel Amthauer. All Rights Reserved
using UnrealBuildTool;
 
public class StaticMeshRuntimeGenerationEditor : ModuleRules
{
	public StaticMeshRuntimeGenerationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new [] { "Core", "CoreUObject", "Engine", "UnrealEd"});
		PrivateDependencyModuleNames.AddRange(new []
		{
			"InputCore",
			"Slate",
			"SlateCore",
			"EditorStyle",
			"ToolMenus",
			"ContentBrowser",
			"StaticMeshRuntimeGeneration"
		});
	}
}