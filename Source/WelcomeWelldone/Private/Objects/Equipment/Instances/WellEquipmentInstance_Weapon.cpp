// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/Equipment/Instances/WellEquipmentInstance_Weapon.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentInstance_Weapon)

void UWellEquipmentInstance_Weapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AmmoRegister);
}

void UWellEquipmentInstance_Weapon::OnEquipped(const UWellEquipmentProfile* OwningProfile)
{
	Super::OnEquipped(OwningProfile);
	BroadcastWeaponDataChanges();
}

void UWellEquipmentInstance_Weapon::OnRep_AmmoRegister()
{
	BroadcastWeaponDataChanges();
}

void UWellEquipmentInstance_Weapon::BroadcastWeaponDataChanges()
{
	if (GetOwnerAsCharacter()->IsLocallyControlled())
	{
		UGameplayMessageSubsystem* MessageBusSubsystem = &UGameplayMessageSubsystem::Get(GetOwner()->GetWorld());
		if (MessageBusSubsystem != nullptr)
		{
			const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message.Instance.Weapon");
			MessageBusSubsystem->BroadcastMessage(MessageTag, FGameplayMessage_WeaponDataChanged(GetRemainingAmmo(), GetTotalAmmo(), DisplayName));
		}
	}
}
