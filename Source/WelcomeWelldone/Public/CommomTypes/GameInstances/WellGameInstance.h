// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WellGameInstance.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

public:
	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void CreateSession();

	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void FindSession();

	UFUNCTION(BlueprintCallable, Category="Game|Sessions")
	void JoinSession();

	UFUNCTION(Exec, BlueprintCallable, Category="Game|Exit")
	virtual void Exit() final;
};
