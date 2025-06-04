// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/WellGameplayAbility_Jump.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"


UWellGameplayAbility_Jump::UWellGameplayAbility_Jump()
{
	//~ Per actor because of input ability replicated events
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UWellGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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
				AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, Handle, GetCurrentActivationInfo().GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnJumpInputReleased);
				Character->LandedDelegate.AddDynamic(this, &ThisClass::OnCharacterLanded);
			}
		}
	}
}

void UWellGameplayAbility_Jump::OnJumpInputReleased()
{
	const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent != nullptr)
	{
		if (ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor()))
			Character->StopJumping();
	}
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(), GetCurrentActivationInfo(), false, false);
}

void UWellGameplayAbility_Jump::OnCharacterLanded(const FHitResult& Hit)
{
	if (!Hit.bBlockingHit) return;
	const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent != nullptr)
	{
		if (ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor()))
		{
			Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnCharacterLanded);
			Character->StopJumping();
		}
	}
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(), GetCurrentActivationInfo(), false, false);
}
