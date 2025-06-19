// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "WellEquipmentComponent.generated.h"


USTRUCT(BlueprintType)
struct FEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FEquipmentEntry() {}
	explicit FEquipmentEntry(int32 InHandle) : Handle(InHandle) {}

public:
	bool IsValid() { return Handle > 0 && Instance; }

public:
	UPROPERTY()
	TSubclassOf<class UWellEquipmentProfile> InstigatorProfile;
	
	UPROPERTY()
	TObjectPtr<class UWellEquipmentInstance> Instance = nullptr;

	UPROPERTY(Transient, NotReplicated)
	int32 Handle = 1;
	
};

USTRUCT(BlueprintType)
struct FEquipmentStorage : public FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	FEquipmentStorage() {}
	explicit FEquipmentStorage(UActorComponent* InOwningComponent)
	{
		OwningComponent = InOwningComponent;
		EntriesStorage.Reserve(2);
	}
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FEquipmentEntry, FEquipmentStorage>(EntriesStorage, DeltaParms, *this);
	}
	
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	
	FEquipmentEntry& AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	void RemoveEntry(UWellEquipmentInstance* EntryInstance);

	FEquipmentEntry& operator[](int32 Index)
	{
		return EntriesStorage[Index];
	}

	FEquipmentEntry& operator[](UWellEquipmentInstance* EntryInstance)
	{
		for (FEquipmentEntry& Entry : EntriesStorage)
		{
			if (Entry.Instance == EntryInstance)
			{
				return Entry;
			}
		}
		static FEquipmentEntry Empty(-1);
		return Empty;
	}

public:
	UPROPERTY()
	TArray<FEquipmentEntry> EntriesStorage;
	
private:
	UPROPERTY()
	TObjectPtr<UActorComponent> OwningComponent = nullptr;
	
};

template<>
struct TStructOpsTypeTraits<FEquipmentStorage> : public TStructOpsTypeTraitsBase2<FEquipmentStorage>
{
	enum { WithNetDeltaSerializer = true };
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WELCOMEWELLDONE_API UWellEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWellEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Equip, meta=(DisplayName="Equip Entry With Adding"))
	UWellEquipmentInstance* AddEntry_ByEquipmentProfile(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Equip, meta=(DisplayName="Unequip Entry With Removing"))
	void RemoveEntry_ByInstance(UWellEquipmentInstance* Instance);

	UFUNCTION(Server, Reliable)
	void TryToEquipEntry_ByHandle(int32 Handle);

	UFUNCTION(BlueprintPure)
	UWellEquipmentInstance* GetFirstEquippedInstance() const;
	UFUNCTION(BlueprintPure)
	int GetFirstEquippedHandle() const;

	bool CanEquip(int32 Handle) const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

private:
	void EquipEntry_ByHandle(int32 Handle);
	void UnequipEntry_ByHandle(int32 Handle);

	void UnequipEntry_ByInstance(UWellEquipmentInstance* Instance);

	UFUNCTION()
	void OnRep_CurrentEquippedHandle();
	
private:
	UPROPERTY(Replicated)
	FEquipmentStorage EquipmentEntries;
	
	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
    bool bIsCharacterEquipped = false;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquippedHandle)
	/* This is something like handle for replication when trying to equip */
	bool bWasTryToEquip = false;
	
};
