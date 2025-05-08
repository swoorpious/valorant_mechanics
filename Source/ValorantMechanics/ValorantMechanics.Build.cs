// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ValorantMechanics : ModuleRules
{
	public ValorantMechanics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});
	}
}
