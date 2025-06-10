// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Common/UWellGameplayAbility_LinkLayer.h"
#include "CommomTypes/WellGameplayTags.h"

UAnimInstance* UWellGameplayAbility_LinkLayer::GetAnimInstanceFromActorInfo() const
{
	UAnimInstance* AnimInstance = nullptr;
	if (const USkeletalMeshComponent* Mesh = GetOwningComponentFromActorInfo())
	{
		AnimInstance = Mesh->GetAnimInstance();
	}
	return AnimInstance;
}
