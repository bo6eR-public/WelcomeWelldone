// Copyright © 2025 bo6eR. All rights reserved.


#include "CommomTypes/Libraries/WellFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellFunctionLibrary)

FGameplayAbilityTargetDataHandle UWellFunctionLibrary::MakeAbilityTargetDataFromAnimInstance(const TSubclassOf<UAnimInstance>& AnimInstance)
{
	FGameplayAbilityTargetData_AnimInstance* TargetData = new FGameplayAbilityTargetData_AnimInstance();
	TargetData->AnimInstance = AnimInstance;
	
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData_AnimInstance>(TargetData));
	
	return Handle;
}

TSubclassOf<UAnimInstance> UWellFunctionLibrary::GetAnimInstanceFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, EConfirmType& ConfirmType)
{
	if (TargetData.IsValid(0))
	{
		FGameplayAbilityTargetData_AnimInstance* ExtractedTargetData = static_cast<FGameplayAbilityTargetData_AnimInstance*>(TargetData.Data[0].Get());
		if (ExtractedTargetData && ExtractedTargetData->AnimInstance)
		{
			ConfirmType = EConfirmType::Successful;
			return ExtractedTargetData->AnimInstance;
		}
	}
	
	ConfirmType = EConfirmType::Failed;
	return nullptr;
}
