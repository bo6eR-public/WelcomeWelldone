// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_Modifier.generated.h"

UENUM(BlueprintType)
enum EModifierStatus
{
	Applied,
	Extended,
	Removed,
};


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_Modifier : public UWellGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifiers")
	FString DisplayName;

public:
	UFUNCTION(BlueprintCallable)
	void BroadcastModifierStatusMessage(const EModifierStatus& Status) const;
	
};
