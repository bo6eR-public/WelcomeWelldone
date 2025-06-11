// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/WellEquipmentInstance.h"
#include "Characters/WellPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentProfile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommomTypes/WellGameplayTags.h"
#include "CommomTypes/Libraries/WellFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentInstance)

bool UWellEquipmentInstance::Initialize(AActor* SourceOwner)
{
	if (SourceOwner && SourceOwner->HasAuthority())
	{
		OwningCharacter = CastChecked<ACharacter>(SourceOwner);
	}
	return OwningCharacter.IsValid();
}

UWorld* UWellEquipmentInstance::GetWorld() const
{
	if (const APawn* OwningPawn = OwningCharacter.Get())
	{
		return OwningPawn->GetWorld();
	}
	return UObject::GetWorld();
}

bool UWellEquipmentInstance::OnEquipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		PlayerCharacter->OverrideInputSettings(OwningProfile->GetInputConfig());
		SendEvent_LinkAnimInstance(OwningProfile->GetAnimationLayer());
		SpawnEquipmentActor(AttachedActorInfo);
		return true;
	}
	return false;
}

bool UWellEquipmentInstance::OnUneqipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		PlayerCharacter->ResetInputSettings(OwningProfile->GetInputConfig());
		SendEvent_LinkAnimInstance(nullptr); //~ Unlink layer
		DestroyEquipmentActor();
		return true;
	}
	return false;
}

void UWellEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, OwningCharacter);
	DOREPLIFETIME(ThisClass, SpawnedActor);
}

void UWellEquipmentInstance::SendEvent_LinkAnimInstance(const TSubclassOf<UAnimInstance>& LinkedInstance)
{
	FGameplayEventData Payload = FGameplayEventData();
	if (OwningCharacter.IsValid() && OwningCharacter->HasAuthority() && LinkedInstance)
	{
		Payload.TargetData = UWellFunctionLibrary::MakeAbilityTargetDataFromAnimInstance(LinkedInstance);
		Payload.EventTag = WellGameplayTags::Event_LinkLayer;
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwningCharacter.Get(), WellGameplayTags::Event_LinkLayer, Payload);
}

void UWellEquipmentInstance::SpawnEquipmentActor(const FAttachedSpawnInfo& AttachInfo)
{
	if (ACharacter* Character = OwningCharacter.Get())
	{
		if (USceneComponent* AttachComponent = Character->GetMesh())
		{
			SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(AttachInfo.Actor, FTransform::Identity, Character);
			SpawnedActor->FinishSpawning(FTransform::Identity, true);
			SpawnedActor->SetActorRelativeTransform(AttachInfo.Transform);

			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepRelativeTransform;
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
