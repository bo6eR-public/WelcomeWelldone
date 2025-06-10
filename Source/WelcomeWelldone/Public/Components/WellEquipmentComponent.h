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
	}
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FEquipmentEntry, FEquipmentStorage>(EntriesStorage, DeltaParms, *this);
	}
	
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	
	FEquipmentEntry& AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	void RemoveEntry(UWellEquipmentInstance* EntryInstance);

private:
	UPROPERTY()
	TArray<FEquipmentEntry> EntriesStorage;

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

	UFUNCTION(BlueprintCallable, Category=Equip)
	UWellEquipmentInstance* EquipEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	UFUNCTION(BlueprintCallable, Category=Unequip)
	void UnequipEntry(UWellEquipmentInstance* Instance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FEquipmentStorage EquipmentEntries;
	
};
