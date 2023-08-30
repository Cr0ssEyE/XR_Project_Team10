// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XR_Project_Team10 : ModuleRules
{
	public XR_Project_Team10(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
