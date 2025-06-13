// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_FromWeapon.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_FromWeapon : public UWellGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=EventData)
	FGameplayAbilityTargetDataHandle GetTargetDataHandleFromAbilitySpec() const;
	
};
