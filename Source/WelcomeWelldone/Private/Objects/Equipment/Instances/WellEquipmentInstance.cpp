// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/Equipment/Instances/WellEquipmentInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Characters/WellPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Objects//Equipment/Profiles/WellEquipmentProfile.h"
#include "CommomTypes/Libraries/WellFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentInstance)

UWellEquipmentInstance::UWellEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UWellEquipmentInstance::GetWorld() const
{
	if (const AActor* OwningActor = GetOwner())
	{
		return OwningActor->GetWorld();
	}
	return UObject::GetWorld();
}

AActor* UWellEquipmentInstance::GetOwner() const
{
	/* Be sure outer is valid */
	return Cast<AActor>(GetOuter());
}

ACharacter* UWellEquipmentInstance::GetOwnerAsCharacter() const
{
	/* Be sure outer is valid */
	return Cast<ACharacter>(GetOuter());
}

void UWellEquipmentInstance::OnEquipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(GetOwner());
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->OverrideInputSettings(OwningProfile->GetInputConfig());
	}
	BP_LinkAnimLayers(OwningProfile->GetAnimationLayer());
	if (PlayerCharacter->HasAuthority())
	{
		GiveEquipmentAbilities();
		SpawnEquipmentActor(AttachedActorInfo);
	}

	bIsEquipped = true;
}

void UWellEquipmentInstance::OnUneqipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(GetOwner());
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->ResetInputSettings(OwningProfile->GetInputConfig());
	}
	BP_LinkAnimLayers(nullptr); //~ Unlink layer
	if (PlayerCharacter->HasAuthority())
	{
		ClearEquipmentAbilities();
		DestroyEquipmentActor();
	}

	bIsEquipped = false;
}

void UWellEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, SpawnedActor);
	DOREPLIFETIME(ThisClass, bIsEquipped);
}

void UWellEquipmentInstance::SpawnEquipmentActor(const FAttachedSpawnInfo& AttachInfo)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (USceneComponent* AttachComponent = Character->GetMesh())
		{
			const FActorSpawnParameters SpawnParams;
			SpawnedActor = GetWorld()->SpawnActor<AActor>(AttachInfo.Actor, FTransform::Identity, SpawnParams);
			SpawnedActor->SetReplicates(true);

			const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
			SpawnedActor->AttachToComponent(AttachComponent, AttachmentRules, AttachInfo.AttachedSocketName);
		}
	}
}

void UWellEquipmentInstance::DestroyEquipmentActor() const
{
	if (SpawnedActor != nullptr)
	{
		SpawnedActor->Destroy();
	}
}

void UWellEquipmentInstance::GiveEquipmentAbilities()
{
	check(GetOwner());
	
	if (const auto AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (!Abilities.IsEmpty())
		{
			for (const FAbilitySet& AbilitySet : Abilities)
			{
				if (AbilitySet.IsValid())
				{
					FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilitySet.Ability, 1);
					AbilitySpec.SourceObject = GetOwner();
					AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.Tag);

					AbilitySpec.GameplayEventData = MakeShared<FGameplayEventData>();
					AbilitySpec.GameplayEventData.Get()->TargetData = UWellFunctionLibrary::MakeAbilityTargetDataFromEquipInstance(this);

					AbilitySpec.SourceObject = this;
					
					AbilitySystem->GiveAbility(AbilitySpec);
				}
			}
		}
	}
}

void UWellEquipmentInstance::ClearEquipmentAbilities() const
{
	check(GetOwner());
	
	if (const auto AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		TArray<FGameplayAbilitySpec> ActiveAbilities = AbilitySystem->GetActivatableAbilities();
		if (ActiveAbilities.IsEmpty()) return;

		for (const FAbilitySet& AbilitySet : Abilities)
		{
			if (!AbilitySet.IsValid()) continue;
			for (const FGameplayAbilitySpec AbilitySpec : ActiveAbilities)
			{
				if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(AbilitySet.Tag))
				{
					AbilitySystem->ClearAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}
