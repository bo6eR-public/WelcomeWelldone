// Copyright © 2025 bo6eR. All rights reserved.


#include "Animations/AnimNotifies/WellAnimNotify_Reload.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommomTypes/WellGameplayTags.h"

void UWellAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	check(MeshComp);

	if (AActor* Owner = MeshComp->GetOwner(); Owner && Owner->HasAuthority())
	{
		FGameplayEventData Payload;
		Payload.EventTag = WellGameplayTags::Event_Reload;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, WellGameplayTags::Event_Reload, Payload);
	}
}
