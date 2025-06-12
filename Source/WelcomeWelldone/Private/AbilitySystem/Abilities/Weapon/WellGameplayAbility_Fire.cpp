// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Weapon/WellGameplayAbility_Fire.h"
#include "Objects/Equipment/Instances/WellEquipmentInstance_Weapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellGameplayAbility_Fire)

FGameplayAbilityTargetDataHandle UWellGameplayAbility_Fire::GetTargetDataHandleFromAbilitySpec() const
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (AbilitySpec != nullptr && AbilitySpec->GameplayEventData.IsValid())
	{
		return AbilitySpec->GameplayEventData.Get()->TargetData;
	}
	return FGameplayAbilityTargetDataHandle();
}
