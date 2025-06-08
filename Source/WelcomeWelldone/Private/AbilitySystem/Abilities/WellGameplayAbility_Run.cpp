// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/WellGameplayAbility_Run.h"
#include "AbilitySystemComponent.h"
#include "CommomTypes/WellGameplayTags.h"


void UWellGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			AbilitySystemComponent->AbilityReplicatedEventDelegate(
				EAbilityGenericReplicatedEvent::InputReleased, Handle,
				GetCurrentActivationInfo().GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnRunInputReleased);
		}
	}
}
void UWellGameplayAbility_Run::OnRunInputReleased()
{
	const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->AbilityReplicatedEventDelegate(
		EAbilityGenericReplicatedEvent::InputReleased, GetCurrentAbilitySpecHandle(),
		GetCurrentActivationInfo().GetActivationPredictionKey()).RemoveAll(this);
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(),
		GetCurrentActivationInfo(), true, false);
}

void UWellGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(WellGameplayTags::State_Run));
	}
}

