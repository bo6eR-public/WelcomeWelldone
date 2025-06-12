// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/Libraries/WellFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellFunctionLibrary)

FGameplayAbilityTargetDataHandle UWellFunctionLibrary::MakeAbilityTargetDataFromEquipInstance(UWellEquipmentInstance* Instance)
{
	FGameplayAbilityTargetData_EquipInstance* TargetData = new FGameplayAbilityTargetData_EquipInstance();
	TargetData->Instance = Instance;
	
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData_EquipInstance>(TargetData));
	
	return Handle;
}

UWellEquipmentInstance* UWellFunctionLibrary::GetEquipInstanceFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, EConfirmType& ConfirmType)
{
	if (TargetData.IsValid(0))
	{
		FGameplayAbilityTargetData_EquipInstance* ExtractedTargetData = static_cast<FGameplayAbilityTargetData_EquipInstance*>(TargetData.Data[0].Get());
		if (ExtractedTargetData && ExtractedTargetData->Instance)
		{
			ConfirmType = EConfirmType::Successful;
			return ExtractedTargetData->Instance;
		}
	}
	
	ConfirmType = EConfirmType::Failed;
	return nullptr;
}
