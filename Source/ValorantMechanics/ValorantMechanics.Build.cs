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
            "EnhancedInput",
            "AnimGraphRuntime",
            "BlueprintGraph",
            "AnimationBlueprintLibrary",
            "UMG",
            "Niagara",
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AnimGraph",
            "AnimGraphRuntime",
            "BlueprintGraph",
            "UnrealEd", // for editor classes
            "Slate",
            "SlateCore",
            "ToolMenus",
            "EditorStyle",
            "AnimGraphRuntime", 
            "Niagara"
        });
        
        OptimizeCode = CodeOptimization.InNonDebugBuilds;
    }
}
