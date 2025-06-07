// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/WellGameplayAbility_Sneak.h"
#include "AbilitySystemComponent.h"
#include "CommomTypes/WellGameplayTags.h"


void UWellGameplayAbility_Sneak::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, Handle, GetCurrentActivationInfo().GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnSneakInputReleased);
	}
}

void UWellGameplayAbility_Sneak::OnSneakInputReleased()
{
	const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent) return;
	
	AbilitySystemComponent->RemoveActiveEffectsWithAppliedTags(FGameplayTagContainer(WellGameplayTags::State_Sneak));

	AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetCurrentAbilitySpecHandle(), GetCurrentActivationInfo().GetActivationPredictionKey()).RemoveAll(this);
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(), GetCurrentActivationInfo(), true, false);
}
