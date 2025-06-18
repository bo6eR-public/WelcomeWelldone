// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WelcomeWelldone : ModuleRules
{
	public WelcomeWelldone(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"GameplayTags", "GameplayTasks", "GameplayAbilities", "PhysicsCore", 
			"NetCore", "UMG",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem", "OnlineSubsystemSteam"
		});
	}
}
