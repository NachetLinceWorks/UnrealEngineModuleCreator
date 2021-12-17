using UnrealBuildTool;

public class EmptyModule : ModuleRules
{
	public EmptyModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// All the modules we want as string
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay"
		});
	}
}
