// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/WellGameplayAbility_Jump.h"
#include "AbilitySystemComponent.h"
#include "CommomTypes/WellGameplayTags.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


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
			if (const ACharacter* Character = CastChecked<ACharacter>(AbilitySystemComponent->GetAvatarActor(), ECastCheckedType::NullAllowed))
			{
				//~ We've got the lenght of forward vector to know that the character is moving forward
				const float LocalMovementDirection = UKismetMathLibrary::Dot_VectorVector
				(
					Character->GetVelocity().GetSafeNormal(),
					Character->GetActorForwardVector()
				);
				return Character->CanJump() && LocalMovementDirection > 0.1f; // 0.1 means character has a little velocity
			}
		}
	}
	return false;
}

void UWellGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
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
}

void UWellGameplayAbility_Jump::OnJumpInputReleased()
{
	const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent != nullptr)
	{
		if (ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor()))
			Character->StopJumping();
	}
	AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetCurrentAbilitySpecHandle(), GetCurrentActivationInfo().GetActivationPredictionKey()).RemoveAll(this);
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(), GetCurrentActivationInfo(), true, false);
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
	EndAbility(GetCurrentAbilitySpecHandle(), AbilitySystemComponent->AbilityActorInfo.Get(), GetCurrentActivationInfo(), true, false);
}

void UWellGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (const auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(WellGameplayTags::State_Jump));
	}
}
