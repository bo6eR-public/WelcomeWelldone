// Copyright © 2025 bo6eR. All rights reserved.


#include "Components/WellEquipmentComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "Objects/WellEquipmentProfile.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentInstance.h"


/////////////////////////~ Equipment Fast Serializer Array And Item ~////////////////////////

UWellEquipmentInstance* FEquipmentStorage::AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	UWellEquipmentProfile* EquipmentCDO = EquipmentProfile.GetDefaultObject();
	if (EquipmentCDO == nullptr)
		return nullptr;

	if (const auto ExtractedInstance = EquipmentCDO->GetDefaultInstance())
	{
		FEquipmentEntry Entry = EntriesStorage.AddDefaulted_GetRef();
		Entry.Instance = ExtractedInstance;
		Entry.InstigatorProfile = EquipmentProfile;

		const auto AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningComponent->GetOwner());
		if (const auto CustomAbilitySystem = Cast<UWellAbilitySystemComponent>(AbilitySystem))
		{
			EquipmentCDO->GetApplyingData()->GiveToAbilitySystemComponent(CustomAbilitySystem);
		}

		MarkItemDirty(Entry);
	}
	return nullptr;
}

void FEquipmentStorage::RemoveEntry(UWellEquipmentInstance* EntryInstance)
{
	for (auto It = EntriesStorage.CreateIterator(); It; ++It)
	{
		const auto Instance = It->Instance;
		if (Instance && Instance == EntryInstance)
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
		if (Entry.Instance)
		{
			Entry.Instance->OnUneqipped();
		}
	}
}

void FEquipmentStorage::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FEquipmentEntry& Entry = EntriesStorage[Index];
		if (Entry.Instance)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

/////////////////////////~ Equipment Component ~////////////////////////

UWellEquipmentComponent::UWellEquipmentComponent() : EquipmentEntries(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

UWellEquipmentInstance* UWellEquipmentComponent::EquipEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	checkf(EquipmentProfile, TEXT("Equipment profile is not valid!"));
	
	if (UWellEquipmentInstance* ResultInstance = EquipmentEntries.AddEntry(EquipmentProfile))
	{
		ResultInstance->OnEquipped();
		return ResultInstance;
	}
	return nullptr;
}

void UWellEquipmentComponent::UnequipEntry(UWellEquipmentInstance* Instance)
{
	if (Instance != nullptr)
	{
		Instance->OnUneqipped();
		EquipmentEntries.RemoveEntry(Instance);
	}
}

void UWellEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, EquipmentEntries);
}
