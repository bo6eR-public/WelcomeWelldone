// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/WellEquipmentProfile.h"
#include "Objects/WellEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentProfile)

UWellEquipmentInstance* UWellEquipmentProfile::GetDefaultInstance() const
{
	return EquipmentInstance.GetDefaultObject();
}
