// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/WellCommonTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellCommonTypes)

bool FGameplayAbilityTargetData_AnimInstance::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Ar << AnimInstance;
	return true;
}
