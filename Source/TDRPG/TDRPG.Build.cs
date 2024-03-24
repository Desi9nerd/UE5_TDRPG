// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TDRPG : ModuleRules
{
	public TDRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "GameplayAbilities", "UMG", "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks", "DatasmithCore", "NavigationSystem", "MotionWarping" }); // Game Ability System(GAS)
    }
}
