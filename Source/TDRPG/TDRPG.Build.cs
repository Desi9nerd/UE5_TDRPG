// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TDRPG : ModuleRules
{
	public TDRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "UMG", "Slate", "SlateCore", "AIModule", "NetCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks", "DatasmithCore", "NavigationSystem", "MotionWarping", "Niagara" }); // Game Ability System(GAS)
    }
}
