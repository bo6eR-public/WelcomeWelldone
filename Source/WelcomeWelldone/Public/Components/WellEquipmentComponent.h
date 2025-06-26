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
	bool IsValid() const { return Handle >= 0 && Instance; }
	class UWellEquipmentProfile* GetProfilePtr() const { return InstigatorProfile.GetDefaultObject(); }

public:
	UPROPERTY()
	TSubclassOf<UWellEquipmentProfile> InstigatorProfile;
	
	UPROPERTY()
	TObjectPtr<class UWellEquipmentInstance> Instance = nullptr;

	UPROPERTY()
	int32 Handle = 0;
	
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
	
	FEquipmentEntry& AddEntry(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	void RemoveEntry(UWellEquipmentInstance* EntryInstance);

	FEquipmentEntry& operator[](int32 Index)
	{
		if (Index >= 0)
		{
			return EntriesStorage[Index];
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

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Equip, meta=(DisplayName="Add Entry"))
	UWellEquipmentInstance* AddEntry_ByEquipmentProfiles(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Equip, meta=(DisplayName="Equip Entry With Adding"))
	UWellEquipmentInstance* EquipEntry_ByEquipmentProfile(const TSubclassOf<UWellEquipmentProfile>& EquipmentProfile);

	UFUNCTION(Server, Reliable)
	void TryToEquipEntry_ByHandle(int32 Handle);
	
	UFUNCTION(BlueprintPure)
	UWellEquipmentInstance* GetFirstEquippedInstance() const;
	int32 GetFirstEquippedHandle() const;

	bool CanEquip(int32 Handle) const;

	UFUNCTION(BlueprintCallable, Category=Equip, meta=(DisplayName="Try To Equip By Key"))
	FORCEINLINE void BP_TryToEquipEntry_ByHandle(int32 Key)
	{
		TryToEquipEntry_ByHandle(Key);
	}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

private:
	void EquipEntry_ByHandle(int32 Handle);
	void UnequipEntry_ByHandle(int32 Handle);

	UFUNCTION()
	void OnRep_bIsCharacterEquipped();

	void SetIsCharacterEquipped(bool bIsEquipped);
	
private:
	UPROPERTY(Replicated)
	FEquipmentStorage EquipmentEntries;
	
	UPROPERTY(ReplicatedUsing=OnRep_bIsCharacterEquipped, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
    bool bIsCharacterEquipped;

	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 PreviousHandle;

	UPROPERTY()
	FTimerHandle PlayRateTimerHandle;
};
