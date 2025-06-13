// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Weapon/WellGameplayAbility_FromWeapon.h"

FGameplayAbilityTargetDataHandle UWellGameplayAbility_FromWeapon::GetTargetDataHandleFromAbilitySpec() const
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (AbilitySpec != nullptr && AbilitySpec->GameplayEventData.IsValid())
	{
		return AbilitySpec->GameplayEventData.Get()->TargetData;
	}
	return FGameplayAbilityTargetDataHandle();
}
