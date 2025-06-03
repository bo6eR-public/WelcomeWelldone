// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "WellCommonStartUpDataAsset.generated.h"


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

UCLASS()
class WELCOMEWELLDONE_API UWellCommonStartUpDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystemComponent(class UWellAbilitySystemComponent* AbilitySystem, int32 ApplyLevel = 1);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<FAbilityActionSet> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

private:
	void GrantAbilities(TArray<FAbilityActionSet> Abilities, UWellAbilitySystemComponent* AbilitySystem, int32 Level);
	void ApplyEffects(TArray<TSubclassOf<UGameplayEffect>> Effects, UWellAbilitySystemComponent* AbilitySystem, int32 Level);
};
