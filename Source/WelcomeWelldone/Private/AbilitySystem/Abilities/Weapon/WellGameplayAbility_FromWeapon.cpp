// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Weapon/WellGameplayAbility_FromWeapon.h"
#include "Objects/Equipment/Instances/WellEquipmentInstance_Weapon.h"

FGameplayAbilityTargetDataHandle UWellGameplayAbility_FromWeapon::GetTargetDataHandleFromAbilitySpec() const
{
	const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	if (AbilitySpec != nullptr && AbilitySpec->GameplayEventData.IsValid())
	{
		return AbilitySpec->GameplayEventData.Get()->TargetData;
	}
	return FGameplayAbilityTargetDataHandle();
}

UWellEquipmentInstance_Weapon* UWellGameplayAbility_FromWeapon::GetAttachmentWeaponInstance() const
{
	return Cast<UWellEquipmentInstance_Weapon>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));
}
