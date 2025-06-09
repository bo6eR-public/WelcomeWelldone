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

public:
	UPROPERTY()
	TSubclassOf<class UWellEquipmentProfile> InstigatorProfile;
	
	UPROPERTY()
	TObjectPtr<class UWellEquipmentInstance> Instance = nullptr;
	
};

USTRUCT(BlueprintType)
struct FEquipmentStorage : FFastArraySerializer
{
	GENERATED_BODY()
	
public:
	FEquipmentStorage(UActorComponent* InOwningComponent)
	{
		OwningComponent = InOwningComponent;
	}
	
protected:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FEquipmentEntry, FEquipmentStorage>(EntriesStorage, DeltaParms, *this);
	}

	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);

public:
	UWellEquipmentInstance* AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
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
	UWellEquipmentComponent();

	UWellEquipmentInstance* EquipEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	void UnequipEntry(UWellEquipmentInstance* Instance);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated)
	FEquipmentStorage EquipmentEntries;
	
};
