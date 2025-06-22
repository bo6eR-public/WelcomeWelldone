// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Online.h"
#include "AbilitySystemGlobals.h"
#include "OnlineSubsystemUtils.h"

UWellGameInstance::UWellGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
{
}

void UWellGameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UWellGameInstance::Server_CreateSession_Implementation()
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		const IOnlineSessionPtr OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();

		const FNamedOnlineSession* ExistedSession = OnlineSessionPtr->GetNamedSession(NAME_GameSession);
		if (ExistedSession != nullptr)
		{
			OnlineSessionPtr->DestroySession(NAME_GameSession);
		}
		
		if (OnlineSessionPtr.IsValid())
		{
			OnlineSessionPtr->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.NumPublicConnections = 2;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUseLobbiesIfAvailable = true;

			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionPtr->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
		}
	}
}

void UWellGameInstance::OnCreateSessionComplete(FName Name, bool bSuccess)
{
	if (bSuccess)
	{
		const FString& LevelPath = TEXT("/Game/Levels/Maps/LV_Master");
		GetWorld()->ServerTravel(LevelPath, TRAVEL_Absolute);
	}
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
