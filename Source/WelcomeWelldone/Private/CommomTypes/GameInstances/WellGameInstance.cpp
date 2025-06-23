// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/GameInstances/WellGameInstance.h"
#include "OnlineSessionSettings.h"
#include "AbilitySystemGlobals.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Online.h"
#include "Kismet/KismetSystemLibrary.h"

UWellGameInstance::UWellGameInstance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), OnCreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,  &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
{
	UniqueSessionName = TEXT("Welldone! Wellcome!");
	UniqueSessionPassword = TEXT("bo6eR");
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
			/* We have to delete existing session for work withou troubles */
			OnlineSessionPtr->DestroySession(NAME_GameSession);
		}
		
		if (OnlineSessionPtr.IsValid())
		{
			OnlineSessionPtr->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			
			TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
			SessionSettings->bIsLANMatch = false;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->NumPublicConnections = 2;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bUsesPresence = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bUseLobbiesIfAvailable = true;
			SessionSettings->bIsDedicated = false;

			SessionSettings->Set(UniqueSessionName, UniqueSessionPassword, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			OnlineSessionPtr->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings.Get());
		}
	}
}

void UWellGameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccess)
{
	if (bSuccess || SessionName == NAME_GameSession)
	{
		GetWorld()->ServerTravel(TEXT("/Game/Levels/Maps/LV_Master?listen"), true);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnCreateSessionComplete"));
	}
}

void UWellGameInstance::FindSession()
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		const IOnlineSessionPtr OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();

		OnlineSessionPtr->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

		SessionSearch = MakeShared<FOnlineSessionSearch>();
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 10;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		OnlineSessionPtr->FindSessions(*GetLocalPlayer()->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	}
}

void UWellGameInstance::OnFindSessionsComplete(bool bSuccess)
{
	const IOnlineSessionPtr OnlineSessionPtr = Online::GetSessionInterface(GetWorld());
	if (bSuccess && OnlineSessionPtr)
	{
		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			if (Result.IsValid())
			{
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

//void UWellGameInstance::JoinSession()
//{
//	
//}

void UWellGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		const IOnlineSessionPtr OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();
		if (Result == EOnJoinSessionCompleteResult::Success && GetWorld())
		{
			const auto ClientController = GetWorld()->GetFirstPlayerController();
			if (ClientController && OnlineSessionPtr.IsValid())
			{
				FString ConnectionInfo;
				OnlineSessionPtr->GetResolvedConnectString(NAME_GameSession, ConnectionInfo);
				ClientController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
			}
		}
	}
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
			UKismetSystemLibrary::QuitGame(World, ClientController, EQuitPreference::Quit, true);
		}
	}
#endif
}
