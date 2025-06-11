// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/WellEquipmentInstance.h"
#include "Characters/WellPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentProfile.h"
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
		PlayerCharacter->Multicast_SendEvent_LinkAnimInstance(OwningProfile->GetAnimationLayer());
		SpawnEquipmentActor(AttachedActorInfo);
		return true;
	}
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->OverrideInputSettings(OwningProfile->GetInputConfig());
		return true;
	}
	return false;
}

bool UWellEquipmentInstance::OnUneqipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		PlayerCharacter->Multicast_SendEvent_LinkAnimInstance(nullptr); //~ Unlink layer
		DestroyEquipmentActor();
		return true;
	}
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->ResetInputSettings(OwningProfile->GetInputConfig());
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
