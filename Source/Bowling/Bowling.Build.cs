// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bowling : ModuleRules
{
	public Bowling(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SlateCore","ApplicationCore"});

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "DUETween" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateIncludePaths.Add("Private/Android");

			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"Launch",
				}
			);
		}
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			bEnableObjCAutomaticReferenceCounting = true;
			PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
			
			PrivateIncludePaths.Add("Private/IOS");
		}
	}
}
