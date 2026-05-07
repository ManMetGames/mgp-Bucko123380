// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Jetpack : ModuleRules
{
	public Jetpack(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Jetpack",
			"Jetpack/Variant_Platforming",
			"Jetpack/Variant_Platforming/Animation",
			"Jetpack/Variant_Combat",
			"Jetpack/Variant_Combat/AI",
			"Jetpack/Variant_Combat/Animation",
			"Jetpack/Variant_Combat/Gameplay",
			"Jetpack/Variant_Combat/Interfaces",
			"Jetpack/Variant_Combat/UI",
			"Jetpack/Variant_SideScrolling",
			"Jetpack/Variant_SideScrolling/AI",
			"Jetpack/Variant_SideScrolling/Gameplay",
			"Jetpack/Variant_SideScrolling/Interfaces",
			"Jetpack/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
