// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WellGameInstance.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWellGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Init() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Session|Data")
	FName UniqueSessionName;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Session|Data")
	FString UniqueSessionPassword;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Game|Sessions")
	void Server_CreateSession();

	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void FindSession();

	//UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	//void JoinSession();

	UFUNCTION(Exec, BlueprintCallable, Category="Game|Exit")
	virtual void Exit() final;

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

private:
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bSuccess);
	UFUNCTION()
	void OnFindSessionsComplete(bool bSuccess);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FORCEINLINE ULocalPlayer* GetLocalPlayer() const { return GetWorld()->GetFirstLocalPlayerFromController(); }
	
};
