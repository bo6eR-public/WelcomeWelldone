// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "WellGameInstance.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWellGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Init() override;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Game|Sessions")
	void Server_CreateSession();

	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void FindSession();

	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void JoinSession();

	UFUNCTION(Exec, BlueprintCallable, Category="Game|Exit")
	virtual void Exit() final;

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

private:
	UFUNCTION()
	void OnCreateSessionComplete(FName Name, bool bSuccess);
};
