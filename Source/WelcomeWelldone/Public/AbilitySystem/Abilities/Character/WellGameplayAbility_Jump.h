// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_Jump.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_Jump : public UWellGameplayAbility
{
	GENERATED_BODY()

public:
	UWellGameplayAbility_Jump();
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	/* Calls by AbilityReplicatedEventDelegate, which is processed in ability system component */
	void OnJumpInputReleased();

	/* Calls by character Landed delegate */
	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
};
