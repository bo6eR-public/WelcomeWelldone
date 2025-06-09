// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellCommonTypes.generated.h"


USTRUCT(BlueprintType)
struct FAbilityActionSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWellGameplayAbility> Ability;

	bool IsValid() const { return Tag.IsValid() && Ability; }
};
