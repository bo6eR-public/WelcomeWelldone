// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_Fire.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_Fire : public UWellGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=EventData)
	FGameplayAbilityTargetDataHandle GetTargetDataHandleFromAbilitySpec() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GameplayCues)
	FGameplayTag GameplayCueTag;
};
