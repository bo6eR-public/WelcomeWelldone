// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "AbilitySystemGlobals.h"
#include "OnlineSubsystemUtils.h"
#include "CommomTypes/WellCommonTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/GameplayMessageSubsystem.h"

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
	if (const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld()))
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
			SessionSettings.NumPrivateConnections = 0;
			SessionSettings.NumPublicConnections = 2;
			SessionSettings.bAllowInvites = true;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = true;
			SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
			SessionSettings.bIsDedicated = false;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bIsLANMatch = true;
			SessionSettings.bShouldAdvertise = true;

			SessionSettings.Set(UniqueSessionName, UniqueSessionPassword, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			
			OnlineSessionPtr->CreateSession(*GetLocalPlayer()->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
		}
	}
}

void UWellGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccessful)
{
	if(const IOnlineSessionPtr OnlineSessionPtr =  Online::GetSessionInterface(GetWorld()))
	{
		FDelegateHandle OnCreateSessionCompleteDelegateHandle = OnCreateSessionCompleteDelegate.GetHandle();
		OnlineSessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		if (bSuccessful && GetWorld())
		{
			// ServerTravel(TEXT("/Game/Levels/Maps/LV_Lobby"));
			UGameplayStatics::OpenLevel(GetWorld(), "LV_Lobby", true, "listen");
		}
	}
}

void UWellGameInstance::ServerTravel_Implementation(const FString& URL, const bool bAbsolute)
{
	GetWorld()->ServerTravel(URL + "?listen", bAbsolute);
	//UGameplayStatics::OpenLevel(GetWorld(), "NewMap", true, "listen");
}

void UWellGameInstance::FindSession()
{
	if (const IOnlineSessionPtr OnlineSessionPtr =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionPtr->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 25;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		OnlineSessionPtr->FindSessions(*GetLocalPlayer()->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
		
		if (UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
		{
			MessageBusSubsystem->BroadcastMessage<FGameplayMessage_BoolData>(FGameplayTag::RequestGameplayTag("Message.Sessions.Finding"),
				FGameplayMessage_BoolData(true));
		}
	}
}

void UWellGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld());
	if (bSuccess && OnlineSessionPtr)
	{
		FDelegateHandle FindSessionsCompleteDelegateHandle = OnFindSessionsCompleteDelegate.GetHandle();
		OnlineSessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
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
		if (UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetWorld()))
		{
			MessageBusSubsystem->BroadcastMessage<FGameplayMessage_BoolData>(FGameplayTag::RequestGameplayTag("Message.Sessions.Finding"),
				FGameplayMessage_BoolData(false));
		}
	}
}

void UWellGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld()))
	{
		FDelegateHandle JoinSessionCompleteDelegateHandle = OnJoinSessionCompleteDelegate.GetHandle();
		OnlineSessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Result %d"), Result));
		if (Result == EOnJoinSessionCompleteResult::Success && GetWorld())
		{
			const auto ClientController = GetWorld()->GetFirstPlayerController();
			if (ClientController && OnlineSessionPtr.IsValid())
			{
				FString ConnectionInfo;
				if (!OnlineSessionPtr->GetResolvedConnectString(NAME_GameSession, ConnectionInfo)) return;
				ClientController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("JOIn %s"), *ConnectionInfo));
			}
		}
	}
}

void UWellGameInstance::DestroySession() const
{
	if (const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld()))
	{
		const FNamedOnlineSession* NamedSession = OnlineSessionPtr->GetNamedSession(NAME_GameSession);
		if (NamedSession != nullptr)
		{
			OnlineSessionPtr->DestroySession(NAME_GameSession);
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
			DestroySession();
			ClientController->ConsoleCommand(FString::Printf(TEXT("quit")));
		}
	}
#endif
}
