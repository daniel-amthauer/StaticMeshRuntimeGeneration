using UnrealBuildTool;
 
public class StaticMeshRuntimeGenerationEditor : ModuleRules
{
	public StaticMeshRuntimeGenerationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd"});
		PrivateDependencyModuleNames.AddRange(new string[] { "InputCore", "Slate", "SlateCore", "EditorStyle", "StaticMeshRuntimeGeneration"});
 
		PublicIncludePaths.AddRange(new string[] {"StaticMeshRuntimeGenerationEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"StaticMeshRuntimeGenerationEditor/Private"});
	}
}