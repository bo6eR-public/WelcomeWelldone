// Copyright © 2025 bo6eR. All rights reserved.


#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "AbilitySystemComponent.h"


void UWellCommonStartUpDataAsset::GiveToAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystem, int32 ApplyLevel)
{
	GrantAbilities(DefaultAbilities, AbilitySystem, ApplyLevel);
	ApplyEffects(DefaultEffects, AbilitySystem, ApplyLevel);
}

void UWellCommonStartUpDataAsset::GrantAbilities(TArray<FAbilitySet> Abilities, UAbilitySystemComponent* AbilitySystem, int32 Level)
{
	if (Abilities.IsEmpty()) return;
	for (const FAbilitySet& AbilitySet : Abilities)
	{
		if (AbilitySet.IsValid())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilitySet.Ability);
			AbilitySpec.SourceObject = AbilitySystem->GetAvatarActor();
			AbilitySpec.Level = Level;
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.Tag);
			
			AbilitySystem->GiveAbility(AbilitySpec);
		}
	}
}

void UWellCommonStartUpDataAsset::ApplyEffects(TArray<TSubclassOf<UGameplayEffect>> Effects, UAbilitySystemComponent* AbilitySystem, int32 Level)
{
	if (Effects.IsEmpty()) return;
	for (TSubclassOf<UGameplayEffect> Effect : Effects)
	{
		if (Effect != nullptr)
		{
			UGameplayEffect* EffectCDO = Effect->GetDefaultObject<UGameplayEffect>();
			AbilitySystem->ApplyGameplayEffectToSelf(EffectCDO, Level, AbilitySystem->MakeEffectContext());
		}
	}
}
