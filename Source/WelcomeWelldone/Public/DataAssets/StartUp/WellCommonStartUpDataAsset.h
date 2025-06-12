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
	void GiveToAbilitySystemComponent(UAbilitySystemComponent* AbilitySystem, UObject* SourceObject, int32 ApplyLevel = 1);
	void TakeFromAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Abilities, meta=(AllowPrivateAccess="true"))
	TArray<FAbilitySet> AbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Abilities, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UGameplayAbility>> OnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effects, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

private:
	void GrantAbilities(TArray<FAbilitySet> Abilities, UAbilitySystemComponent* AbilitySystem, UObject* SourceObject, int32 Level);
	void GrantAbilities(TArray<TSubclassOf<UGameplayAbility>>& Abilities, UAbilitySystemComponent* AbilitySystem, UObject* SourceObject, int32 Level);
	
	void ApplyEffects(TArray<TSubclassOf<UGameplayEffect>> Effects, UAbilitySystemComponent* AbilitySystem, int32 Level);

	void ClearAbilities(TArray<FAbilitySet> Abilities, UAbilitySystemComponent* AbilitySystem);
};
