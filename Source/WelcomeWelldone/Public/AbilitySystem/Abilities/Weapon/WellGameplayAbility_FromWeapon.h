// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_FromWeapon.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_FromWeapon : public UWellGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Instance)
	USkeletalMeshComponent* TryToExtractMeshFromSpawnedActor() const;

public:
	/* Deprecated because, when we're calling activate ability in the second time, target data is reseted */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=EventData)
	FGameplayAbilityTargetDataHandle GetTargetDataHandleFromAbilitySpec() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Instance)
	class UWellEquipmentInstance_Weapon* GetAttachmentWeaponInstance() const;
	
};
