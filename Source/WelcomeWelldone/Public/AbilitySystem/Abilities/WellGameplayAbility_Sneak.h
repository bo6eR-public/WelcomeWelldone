// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WellGameplayAbility.h"
#include "WellGameplayAbility_Sneak.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_Sneak : public UWellGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	/* Calls by AbilityReplicatedEventDelegate, which is processed in ability system component */
	void OnSneakInputReleased();
};
