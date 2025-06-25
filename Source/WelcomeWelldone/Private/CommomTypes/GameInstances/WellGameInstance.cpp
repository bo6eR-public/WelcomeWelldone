// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include "AbilitySystemGlobals.h"
#include "OnlineSubsystemUtils.h"
#include "Online.h"

UWellGameInstance::UWellGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,  &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{
	UniqueSessionName = TEXT("WellcomeWelldone");
	UniqueSessionPassword = TEXT("bo6eR");
}

void UWellGameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

void UWellGameInstance::Server_CreateSession_Implementation()
{
	if (const IOnlineSessionPtr OnlineSessionPtr =  Online::GetSessionInterface(GetWorld()))
	{
		if (OnlineSessionPtr->GetNamedSession(NAME_GameSession))
		{
			/* We have to delete existing session for work without troubles */
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
			SessionSettings.bIsDedicated = false;

			SessionSettings.Set(UniqueSessionName, UniqueSessionPassword, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			
			OnlineSessionPtr->CreateSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
			OnlineSessionPtr->UpdateSession(NAME_GameSession, SessionSettings, true);
		}
	}
}

void UWellGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccessful)
{
	if(const IOnlineSessionPtr OnlineSessionPtr =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		if (bSuccessful && OnlineSessionPtr->StartSession(NAME_GameSession))
		{
			GetWorld()->ServerTravel(TEXT("/Game/Levels/Maps/LV_Lobby?listen"), true);
		}
	}
}

void UWellGameInstance::ServerTravel_Implementation(const FString& URL)
{
	GetWorld()->ServerTravel(URL + "?listen", true);
}

void UWellGameInstance::FindSession()
{
	if (const IOnlineSessionPtr OnlineSessionPtr =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionPtr->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 250;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		OnlineSessionPtr->FindSessions(*GetLocalPlayer()->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	}
}

void UWellGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld());
	if (bSuccess && OnlineSessionPtr)
	{
		OnlineSessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			if (Result.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Player: %s"), *Result.Session.OwningUserName));
				FString ExtractedPassword;
				Result.Session.SessionSettings.Get(UniqueSessionName, ExtractedPassword);
				if (ExtractedPassword == UniqueSessionPassword)
				{
					OnlineSessionPtr->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
					OnlineSessionPtr->JoinSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, Result);
					break;
				}
			}
		}
	}
}

void UWellGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionPtr->ClearOnJoinSessionCompleteDelegates(this);
		
		if (Result == EOnJoinSessionCompleteResult::Success && GetWorld())
		{
			const auto ClientController = GetWorld()->GetFirstPlayerController();
			if (ClientController && OnlineSessionPtr.IsValid())
			{
				FString ConnectionInfo;
				OnlineSessionPtr->GetResolvedConnectString(NAME_GameSession, ConnectionInfo);
				ClientController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("JOIn %s"), *ConnectionInfo));
			}
		}
	}
}

void UWellGameInstance::Exit()
{
	UWorld* World = GetWorld();
	check(World);

#if PLATFORM_WINDOWS
	const ENetMode NetMode = World->GetNetMode();
	if (NetMode == NM_Client || NetMode == NM_ListenServer || NetMode == NM_Standalone)
	{
		if (const auto ClientController = World->GetFirstPlayerController())
		{
			ClientController->ConsoleCommand(FString::Printf(TEXT("quit")));
		}
	}
#endif
}
