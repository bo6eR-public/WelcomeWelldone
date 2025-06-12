// Copyright © 2025 bo6eR. All rights reserved.


#include "Components/WellEquipmentComponent.h"
#include "Engine/ActorChannel.h"
#include "Objects/Equipment/Profiles/WellEquipmentProfile.h"
#include "Objects/Equipment/Instances/WellEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

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

void FEquipmentStorage::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FEquipmentEntry& Entry = EntriesStorage[Index];
		if (Entry.IsValid())
		{
			const auto OwningProfile = Entry.InstigatorProfile;
			Entry.Instance->OnUneqipped(OwningProfile.GetDefaultObject());
		}
	}
}

void FEquipmentStorage::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FEquipmentEntry& Entry = EntriesStorage[Index];
		if (Entry.IsValid())
		{
			const auto OwningProfile = Entry.InstigatorProfile;
			Entry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
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

UWellEquipmentInstance* UWellEquipmentComponent::EquipEntry_ByEquipmentProfile(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	FEquipmentEntry& ResultEntry = EquipmentEntries.AddEntry(EquipmentProfile);
	
	if (ResultEntry.IsValid())
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(ResultEntry.Instance);
		}
		const auto OwningProfile = ResultEntry.InstigatorProfile;
		ResultEntry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
		
		bIsCharacterEquipped = true;
		return ResultEntry.Instance;
	}
	
	return nullptr;
}

void UWellEquipmentComponent::EquipEntry_ByHandle(int32 Handle)
{
	FEquipmentEntry& Entry = EquipmentEntries[Handle];
	if (Entry.IsValid())
	{
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(Entry.Instance);
		}
		const auto OwningProfile = Entry.InstigatorProfile;
		Entry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
		
		bIsCharacterEquipped = true;
	}
}

void UWellEquipmentComponent::UnequipEntry_ByHandle(int32 Handle)
{
	FEquipmentEntry& Entry = EquipmentEntries[Handle];
	if (Entry.IsValid())
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(Entry.Instance);
		}
		const auto OwningProfile = Entry.InstigatorProfile;
		Entry.Instance->OnUneqipped(OwningProfile.GetDefaultObject());
			
		bIsCharacterEquipped = false;
		EquipmentEntries.RemoveEntry(Entry.Instance);
	}
}

void UWellEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, EquipmentEntries);
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
