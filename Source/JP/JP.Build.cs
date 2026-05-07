// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JP : ModuleRules
{
	public JP(ReadOnlyTargetRules Target) : base(Target)
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
			"JP",
			"JP/Variant_Platforming",
			"JP/Variant_Platforming/Animation",
			"JP/Variant_Combat",
			"JP/Variant_Combat/AI",
			"JP/Variant_Combat/Animation",
			"JP/Variant_Combat/Gameplay",
			"JP/Variant_Combat/Interfaces",
			"JP/Variant_Combat/UI",
			"JP/Variant_SideScrolling",
			"JP/Variant_SideScrolling/AI",
			"JP/Variant_SideScrolling/Gameplay",
			"JP/Variant_SideScrolling/Interfaces",
			"JP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
