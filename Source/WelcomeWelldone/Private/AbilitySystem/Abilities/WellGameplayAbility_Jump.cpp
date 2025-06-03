// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/WellGameplayAbility_Jump.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"


bool UWellGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			const ACharacter* Character = CastChecked<ACharacter>(AbilitySystemComponent->GetAvatarActor(), ECastCheckedType::NullAllowed);
			return Character ? Character->CanJump() : false;
		}
	}
	return false;
}

void UWellGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			if (ACharacter* Character = CastChecked<ACharacter>(
				AbilitySystemComponent->GetAvatarActor(), ECastCheckedType::NullAllowed))
			{
				Character->Jump();
				//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			}
		}
	}
}

void UWellGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		if (ACharacter* Character = CastChecked<ACharacter>(
			AbilitySystemComponent->GetAvatarActor(), ECastCheckedType::NullAllowed))
			Character->StopJumping();
	}
}
