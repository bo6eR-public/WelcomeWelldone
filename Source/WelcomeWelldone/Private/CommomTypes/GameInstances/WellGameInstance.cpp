// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "AbilitySystemGlobals.h"

void UWellGameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UWellGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("CREATE"));
}

void UWellGameInstance::FindSession()
{
	UE_LOG(LogTemp, Warning, TEXT("FIND"));
}

void UWellGameInstance::JoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("JOIN"));
}

void UWellGameInstance::Exit()
{
	UWorld* World = GetWorld();
	check(World);
	
#if PLATFORM_WINDOWS
	if (World->GetNetMode() == NM_Client)
	{
		if (const auto ClientController = World->GetFirstPlayerController())
		{
			ClientController->ConsoleCommand(TEXT("Quit"));
		}
	}
#endif
}
