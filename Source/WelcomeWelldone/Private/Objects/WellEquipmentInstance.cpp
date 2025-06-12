// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/WellEquipmentInstance.h"
#include "Characters/WellPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentProfile.h"
#include "CommomTypes/Libraries/WellFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentInstance)

UWellEquipmentInstance::UWellEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

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

void UWellEquipmentInstance::OnEquipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->OverrideInputSettings(OwningProfile->GetInputConfig());
	}
	LinkAnimLayers(OwningProfile->GetAnimationLayer());
	if (PlayerCharacter->HasAuthority())
	{
		SpawnEquipmentActor(AttachedActorInfo);
	}
}

void UWellEquipmentInstance::OnUneqipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->ResetInputSettings(OwningProfile->GetInputConfig());
	}
	LinkAnimLayers(nullptr); //~ Unlink layer
	if (PlayerCharacter->HasAuthority())
	{
		DestroyEquipmentActor();
	}
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
