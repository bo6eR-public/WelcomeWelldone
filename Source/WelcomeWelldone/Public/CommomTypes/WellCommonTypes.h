// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellCommonTypes.generated.h"


UENUM(BlueprintType)
enum class EConfirmType : uint8
{
	Successful,
	Failed,
};

USTRUCT(BlueprintType)
struct FAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWellGameplayAbility> Ability;

	bool IsValid() const { return Tag.IsValid() && Ability; }
};

USTRUCT(BlueprintType)
struct FPlayerInputConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories="Input"))
	FGameplayTag ActionTag;

	bool IsValid() const{ return InputAction && ActionTag.IsValid(); }
};

USTRUCT(BlueprintType)
struct FAttachedSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> Actor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform Transform = FTransform();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachedSocketName = NAME_None;
	
};

USTRUCT()
struct FGameplayAbilityTargetData_AnimInstance : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UAnimInstance> AnimInstance = nullptr;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_AnimInstance::StaticStruct();
	}
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_AnimInstance> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_AnimInstance>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
