// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MessageBusSubsystem : ModuleRules
{
	public MessageBusSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "GameplayTags", "Engine"
		});
	}
}
