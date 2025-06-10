// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/WellEquipmentInstance.h"
#include "Characters/WellPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentProfile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommomTypes/WellGameplayTags.h"
#include "CommomTypes/Libraries/WellFunctionLibrary.h"


bool UWellEquipmentInstance::Initialize(AActor* SourceOwner)
{
	if (SourceOwner && SourceOwner->HasAuthority())
	{
		OwningCharacter = CastChecked<ACharacter>(SourceOwner);
	}
	return OwningCharacter.IsValid();
}

void UWellEquipmentInstance::OnEquipped(const UWellEquipmentProfile* OwningProfile)
{
	AWellPlayerCharacter* PlayerCharacter = Cast<AWellPlayerCharacter>(OwningCharacter.Get());
	if (PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		PlayerCharacter->OverrideInputSettings(OwningProfile->GetInputConfig());

		FGameplayEventData Payload;
		Payload.TargetData = UWellFunctionLibrary::MakeAbilityTargetDataFromAnimInstance(OwningProfile->GetAnimationLayer());
		Payload.EventTag = WellGameplayTags::Event_LinkLayer;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, WellGameplayTags::Event_LinkLayer, Payload);
	}
}

void UWellEquipmentInstance::OnUneqipped()
{
	
}

void UWellEquipmentInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, OwningCharacter);
}
