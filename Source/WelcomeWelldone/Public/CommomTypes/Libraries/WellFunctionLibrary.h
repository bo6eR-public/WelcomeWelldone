// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellFunctionLibrary.generated.h"

/**
 * Library is for helper functions aka GetSpecialTargetData() or smth like this
 */
UCLASS()
class WELCOMEWELLDONE_API UWellFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayAbilityTargetDataHandle MakeAbilityTargetDataFromAnimInstance(const TSubclassOf<UAnimInstance>& AnimInstance);

	UFUNCTION(BlueprintCallable, Category="Storage|Text", meta=(ExpandEnumAsExecs="ConfirmType"))
	static TSubclassOf<UAnimInstance> GetAnimInstanceFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData, EConfirmType& ConfirmType);
};
