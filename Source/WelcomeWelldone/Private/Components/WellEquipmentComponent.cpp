// Copyright © 2025 bo6eR. All rights reserved.


#include "Components/WellEquipmentComponent.h"
#include "Engine/ActorChannel.h"
#include "Objects/Equipment/Profiles/WellEquipmentProfile.h"
#include "Objects/Equipment/Instances/WellEquipmentInstance.h"
#include "Net/UnrealNetwork.h"
//#include "Net/Core/PushModel/PushModel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentComponent)

/////////////////////////~ Equipment Fast Serializer Array And Item ~////////////////////////

FEquipmentEntry& FEquipmentStorage::AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	static FEquipmentEntry EmptyEntry(-1);
	check(OwningComponent);
	check(OwningComponent->GetOwner()->HasAuthority())
	
	UWellEquipmentProfile* EquipmentCDO = EquipmentProfile.GetDefaultObject();
	if (EquipmentCDO == nullptr)
		return EmptyEntry;

	if (const auto ExtractedInstance = EquipmentCDO->EquipmentInstance)
	{
		FEquipmentEntry& Entry = EntriesStorage.AddDefaulted_GetRef();
		Entry.Handle = EntriesStorage.Num() - 1;
		Entry.InstigatorProfile = EquipmentProfile;
		Entry.Instance = NewObject<UWellEquipmentInstance>(OwningComponent->GetOwner(), ExtractedInstance);
		
		MarkItemDirty(Entry);
		return Entry;
	}
	return EmptyEntry;
}

void FEquipmentStorage::RemoveEntry(UWellEquipmentInstance* EntryInstance)
{
	check(OwningComponent);
	check(OwningComponent->GetOwner()->HasAuthority());
	
	for (auto It = EntriesStorage.CreateIterator(); It; ++It)
	{
		FEquipmentEntry& Entry = *It;
		if (Entry.IsValid() && Entry.Instance == EntryInstance)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

/////////////////////////~ Equipment Component ~////////////////////////

UWellEquipmentComponent::UWellEquipmentComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), EquipmentEntries(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

UWellEquipmentInstance* UWellEquipmentComponent::AddEntry_ByEquipmentProfiles(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	FEquipmentEntry& ResultEntry = EquipmentEntries.AddEntry(EquipmentProfile);
	if (ResultEntry.IsValid())
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(ResultEntry.Instance);
		}
		return ResultEntry.Instance;
	}
	return nullptr;
}

UWellEquipmentInstance* UWellEquipmentComponent::EquipEntry_ByEquipmentProfile(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	FEquipmentEntry& ResultEntry = EquipmentEntries.AddEntry(EquipmentProfile);
	if (ResultEntry.IsValid())
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(ResultEntry.Instance);
		}
		TryToEquipEntry_ByHandle(ResultEntry.Handle);
		return ResultEntry.Instance;
	}
	
	return nullptr;
}

void UWellEquipmentComponent::TryToEquipEntry_ByHandle_Implementation(int32 Handle)
{
	if (!CanEquip(Handle)) return;

	PreviousHandle = GetFirstEquippedHandle();
	if (bIsCharacterEquipped)
	{
		/* If character try to equip the same entry */
		if (Handle == GetFirstEquippedHandle())
		{
			UnequipEntry_ByHandle(Handle);
		}
		/* If character try to equip other entry */
		else
		{
			const float UnequipPlayRate = GetFirstEquippedInstance() ? GetFirstEquippedInstance()->GetUnequipPlayRate() : 1.f;
			UnequipEntry_ByHandle(GetFirstEquippedHandle());
			
			GetWorld()->GetTimerManager().SetTimer(PlayRateTimerHandle, FTimerDelegate::CreateLambda([this, Handle]()
			{
				EquipEntry_ByHandle(Handle);
				GetWorld()->GetTimerManager().ClearTimer(PlayRateTimerHandle);
			}), UnequipPlayRate, false);
		}
	}
	else
	{
		/* Flip-flop entries */
		EquipEntry_ByHandle(Handle);
	}
}

void UWellEquipmentComponent::EquipEntry_ByHandle(int32 Handle)
{
	const FEquipmentEntry& Entry = EquipmentEntries[Handle];
	if (Entry.IsValid())
	{
		Entry.Instance->OnEquipped(Entry.GetProfilePtr());
		SetIsCharacterEquipped(true);
	}
}

void UWellEquipmentComponent::UnequipEntry_ByHandle(int32 Handle)
{
	const FEquipmentEntry& Entry = EquipmentEntries[Handle];
	if (Entry.IsValid())
	{
		Entry.Instance->OnUneqipped(Entry.GetProfilePtr());
		SetIsCharacterEquipped(false);
	}
}

void UWellEquipmentComponent::OnRep_bIsCharacterEquipped()
{
	if (bIsCharacterEquipped)
	{
		const FEquipmentEntry& Entry = EquipmentEntries[GetFirstEquippedHandle()];
		if (Entry.IsValid())
		{
			Entry.Instance->OnEquipped(Entry.GetProfilePtr());
		}
	}
	else
	{
		const FEquipmentEntry& Entry = EquipmentEntries[PreviousHandle];
		if (Entry.IsValid())
		{
			Entry.Instance->OnUneqipped(Entry.GetProfilePtr());
		}
	}
}

void UWellEquipmentComponent::SetIsCharacterEquipped(const bool bIsEquipped)
{
	bIsCharacterEquipped = bIsEquipped;
	//MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, bIsCharacterEquipped, this);
}

UWellEquipmentInstance* UWellEquipmentComponent::GetFirstEquippedInstance() const
{
	for (FEquipmentEntry Entry : EquipmentEntries.EntriesStorage)
	{
		if (Entry.IsValid() && Entry.Instance->IsEquipped())
		{
			return Entry.Instance;
		}
	}
	return nullptr;
}

int UWellEquipmentComponent::GetFirstEquippedHandle() const
{
	int Handle = 0;
	for (FEquipmentEntry Entry : EquipmentEntries.EntriesStorage)
	{
		if (Entry.IsValid() && Entry.Instance->IsEquipped())
		{
			return Handle;
		}
		Handle++;
	}
	return -1;
}

bool UWellEquipmentComponent::CanEquip(int32 Handle) const
{
	/* If character has one item, but handle calls the second we exit from function */
	return Handle < EquipmentEntries.EntriesStorage.Num();
}

void UWellEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, EquipmentEntries);
	DOREPLIFETIME(ThisClass, PreviousHandle);

	/*
	FDoRepLifetimeParams RepLifetimeParams;
	RepLifetimeParams.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bIsCharacterEquipped, RepLifetimeParams);
	*/
	DOREPLIFETIME(ThisClass, bIsCharacterEquipped);
}

bool UWellEquipmentComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (FEquipmentEntry& Entry : EquipmentEntries.EntriesStorage)
	{
		UWellEquipmentInstance* Instance = Entry.Instance;
		if (IsValid(Instance))
		{
			bWroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}
	return bWroteSomething;
}

void UWellEquipmentComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FEquipmentEntry& Entry : EquipmentEntries.EntriesStorage)
		{
			UWellEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}
