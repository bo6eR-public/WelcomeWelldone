// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellEquipmentInstance.generated.h"


class UWellEquipmentProfile;

UCLASS(Blueprintable, Abstract)
class WELCOMEWELLDONE_API UWellEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UWellEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UWorld* GetWorld() const override;
	AActor* GetOwner() const;

	UFUNCTION(BlueprintPure)
	ACharacter* GetOwnerAsCharacter() const;
	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetSpawnedActor() const { return SpawnedActor; };
	
	virtual void OnEquipped(const UWellEquipmentProfile* OwningProfile);
	virtual void OnUneqipped(const UWellEquipmentProfile* OwningProfile);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="Link Anim Layers")
	void BP_LinkAnimLayers(TSubclassOf<UAnimInstance> LinkedInstance);

	inline bool IsEquipped() const { return bIsEquipped; };

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SpawnEquipmentActor(const FAttachedSpawnInfo& AttachInfo);
	void DestroyEquipmentActor() const;

	void GiveEquipmentAbilities();
	void ClearEquipmentAbilities() const;

	virtual bool IsSupportedForNetworking() const override { return true; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Equip", meta=(AllowPrivateAccess=true))
	UAnimMontage* EquipMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Unequip", meta=(AllowPrivateAccess=true))
	UAnimMontage* UnequipMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FAttachedSpawnInfo AttachedActorInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Ability, meta=(AllowPrivateAccess="true"))
	TArray<FAbilitySet> Abilities;

private:
	UPROPERTY(Replicated)
	TObjectPtr<AActor> SpawnedActor = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsEquipped = false;
};
