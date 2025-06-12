// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Abilities/Common/UWellGameplayAbility_LinkLayer.h"

UWellGameplayAbility_LinkLayer::UWellGameplayAbility_LinkLayer()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

UAnimInstance* UWellGameplayAbility_LinkLayer::GetAnimInstanceFromActorInfo() const
{
	UAnimInstance* AnimInstance = nullptr;
	if (const USkeletalMeshComponent* Mesh = GetOwningComponentFromActorInfo())
	{
		AnimInstance = Mesh->GetAnimInstance();
	}
	return AnimInstance;
}
