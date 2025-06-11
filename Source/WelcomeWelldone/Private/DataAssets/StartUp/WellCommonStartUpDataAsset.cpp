// Copyright © 2025 bo6eR. All rights reserved.


#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellCommonStartUpDataAsset)

void UWellCommonStartUpDataAsset::GiveToAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystem, int32 ApplyLevel)
{
	GrantAbilities(AbilitySets, AbilitySystem, ApplyLevel);
	ApplyEffects(DefaultEffects, AbilitySystem, ApplyLevel);
	
	GrantAbilities(TriggeredAbilities, AbilitySystem, ApplyLevel);
	GrantAbilities(OnGivenAbilities, AbilitySystem, ApplyLevel);
}

void UWellCommonStartUpDataAsset::TakeFromAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent)
{
	ClearAbilities(AbilitySets, AbilitySystemComponent);
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

void UWellCommonStartUpDataAsset::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>>& Abilities, UAbilitySystemComponent* AbilitySystem, int32 Level)
{
	if (Abilities.IsEmpty()) return;
	for (const auto Ability : Abilities)
	{
		if (Ability != nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
			AbilitySpec.SourceObject = AbilitySystem->GetAvatarActor();
			AbilitySpec.Level = Level;
			
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

void UWellCommonStartUpDataAsset::ClearAbilities(TArray<FAbilitySet> Abilities, UAbilitySystemComponent* AbilitySystem)
{
	TArray<FGameplayAbilitySpec> ActiveAbilities = AbilitySystem->GetActivatableAbilities();
	if (Abilities.IsEmpty() || ActiveAbilities.IsEmpty()) return;

	for (const FAbilitySet& AbilitySet : Abilities)
	{
		if (!AbilitySet.IsValid()) continue;

		for (const FGameplayAbilitySpec AbilitySpec : ActiveAbilities)
		{
			if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(AbilitySet.Tag))
			{
				AbilitySystem->ClearAbility(AbilitySpec.Handle);
			}
		}
	}
}
