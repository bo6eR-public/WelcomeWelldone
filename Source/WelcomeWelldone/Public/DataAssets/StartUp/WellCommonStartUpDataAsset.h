// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommomTypes/WellCommonTypes.h"
#include "Engine/DataAsset.h"
#include "WellCommonStartUpDataAsset.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellCommonStartUpDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystem, int32 ApplyLevel = 1);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<FAbilitySet> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

private:
	void GrantAbilities(TArray<FAbilitySet> Abilities, UAbilitySystemComponent* AbilitySystem, int32 Level);
	void ApplyEffects(TArray<TSubclassOf<UGameplayEffect>> Effects, UAbilitySystemComponent* AbilitySystem, int32 Level);
};
