// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/Equipment/Instances/WellEquipmentInstance_Weapon.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentInstance_Weapon)

void UWellEquipmentInstance_Weapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AmmoRegister);
}
