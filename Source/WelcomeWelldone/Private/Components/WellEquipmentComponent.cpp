// Copyright © 2025 bo6eR. All rights reserved.


#include "Components/WellEquipmentComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "Objects/WellEquipmentProfile.h"
#include "Net/UnrealNetwork.h"
#include "Objects/WellEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellEquipmentComponent)

/////////////////////////~ Equipment Fast Serializer Array And Item ~////////////////////////

FEquipmentEntry& FEquipmentStorage::AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile)
{
	UWellEquipmentProfile* EquipmentCDO = EquipmentProfile.GetDefaultObject();
	static FEquipmentEntry EmptyEntry(-1);
	if (EquipmentCDO == nullptr)
		return EmptyEntry;

	if (const auto ExtractedInstance = EquipmentCDO->GetDefaultInstance())
	{
		FEquipmentEntry& Entry = EntriesStorage.AddDefaulted_GetRef();
		Entry.Instance = ExtractedInstance;
		Entry.InstigatorProfile = EquipmentProfile;
		
		if (const auto AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningComponent->GetOwner()))
		{
			if (const auto StartUpData = EquipmentCDO->GetApplyingAbilityData())
			{
				StartUpData->GiveToAbilitySystemComponent(AbilitySystem);
			}
		}
		MarkItemDirty(Entry);
		return Entry;
	}
	return EmptyEntry;
}

void FEquipmentStorage::RemoveEntry(UWellEquipmentInstance* EntryInstance)
{
	for (auto It = EntriesStorage.CreateIterator(); It; ++It)
	{
		const auto Instance = It->Instance;
		if (Instance && Instance == EntryInstance)
		{
			UWellEquipmentProfile* EquipmentCDO = It->InstigatorProfile.GetDefaultObject();
			if (const auto AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningComponent->GetOwner()))
			{
				if (const auto StartUpData = EquipmentCDO->GetApplyingAbilityData())
				{
					StartUpData->TakeFromAbilitySystemComponent(AbilitySystem);
				}
			}
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
			if (Entry.Instance->Initialize(OwningComponent->GetOwner()))
			{
				Entry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
			}
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
	checkf(EquipmentProfile, TEXT("Equipment profile is not valid!"));
	if (GetOwner()->HasAuthority())
	{
		FEquipmentEntry& ResultEntry = EquipmentEntries.AddEntry(EquipmentProfile);
		if (ResultEntry.IsValid() && ResultEntry.Instance->Initialize(GetOwner()))
		{
			const auto OwningProfile = ResultEntry.InstigatorProfile;
			bIsCharacterEquipped = ResultEntry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
			
			return ResultEntry.Instance;
		}
	}
	return nullptr;
}

void UWellEquipmentComponent::EquipEntry_ByHandle(int32 Handle)
{
	if (GetOwner()->HasAuthority())
	{
		FEquipmentEntry& Entry = EquipmentEntries[Handle];
		if (Entry.IsValid() && Entry.Instance->Initialize(GetOwner()))
		{
			const auto OwningProfile = Entry.InstigatorProfile;
			bIsCharacterEquipped = Entry.Instance->OnEquipped(OwningProfile.GetDefaultObject());
		}
	}
}

void UWellEquipmentComponent::UnequipEntry_ByEquipmentInstance(UWellEquipmentInstance* Instance)
{
	if (GetOwner()->HasAuthority())
	{
		if (Instance != nullptr)
		{
			bIsCharacterEquipped = !Instance->OnUneqipped(nullptr); // TODO: not nullptr
			EquipmentEntries.RemoveEntry(Instance);
		}
	}
}

void UWellEquipmentComponent::UnequipEntry_ByHandle(int32 Handle)
{
	if (GetOwner()->HasAuthority())
	{
		FEquipmentEntry& Entry = EquipmentEntries[Handle];
		if (Entry.IsValid() && Entry.Instance->Initialize(GetOwner()))
		{
			const auto OwningProfile = Entry.InstigatorProfile;
			bIsCharacterEquipped = !Entry.Instance->OnUneqipped(OwningProfile.GetDefaultObject());
		}
		EquipmentEntries.RemoveEntry(Entry.Instance);
	}
}

void UWellEquipmentComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, EquipmentEntries);
}
