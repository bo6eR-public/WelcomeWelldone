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

		FGameplayEventData Payload;
		Payload.TargetData = UWellFunctionLibrary::MakeAbilityTargetDataFromAnimInstance(OwningProfile->GetAnimationLayer());
		Payload.EventTag = WellGameplayTags::Event_LinkLayer;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, WellGameplayTags::Event_LinkLayer, Payload);

		SpawnEquipmentActor(AttachedActorInfo);
		return true;
	}
	return false;
}

void UWellEquipmentInstance::OnUneqipped()
{
	
}

void UWellEquipmentInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, OwningCharacter);
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

bool UWellEquipmentInstance::DestroyEquipmentActor() const
{
	if (SpawnedActor != nullptr)
	{
		SpawnedActor->Destroy();
		return true;
	}
	return false;
}
