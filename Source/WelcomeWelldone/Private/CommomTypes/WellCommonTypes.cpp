// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/WellCommonTypes.h"
#include "Objects/Equipment/Instances/WellEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellCommonTypes)

bool FGameplayAbilityTargetData_EquipInstance::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Ar << Instance;
	return true;
}
