// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GP4_Team7 : ModuleRules
{
	public GP4_Team7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
			"GameplayAbilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks"});

		PublicIncludePaths.AddRange(new string[] {
			"GP4_Team7",
			"GP4_Team7/Variant_Strategy",
			"GP4_Team7/Variant_Strategy/UI",
			"GP4_Team7/Variant_TwinStick",
			"GP4_Team7/Variant_TwinStick/AI",
			"GP4_Team7/Variant_TwinStick/Gameplay",
			"GP4_Team7/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
