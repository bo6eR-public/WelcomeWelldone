// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WellCommonStartUpDataAsset.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellCommonStartUpDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystemComponent(class UWellAbilitySystemComponent* AbilitySystem, int32 ApplyLevel = 1);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<class UWellGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

private:
	void GrantAbilities(TArray<TSubclassOf<UWellGameplayAbility>> Abilities, UWellAbilitySystemComponent* AbilitySystem, int32 Level);
	void ApplyEffects(TArray<TSubclassOf<UGameplayEffect>> Effects, UWellAbilitySystemComponent* AbilitySystem, int32 Level);
};
