// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellEquipmentInstance.generated.h"


class UWellEquipmentProfile;

UCLASS(Blueprintable)
class WELCOMEWELLDONE_API UWellEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	/* Should call from equipment component every time */
	virtual bool Initialize(AActor* SourceOwner);
	virtual UWorld* GetWorld() const override;
	
	virtual bool OnEquipped(const UWellEquipmentProfile* OwningProfile);
	virtual bool OnUneqipped(const UWellEquipmentProfile* OwningProfile);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FAttachedSpawnInfo AttachedActorInfo;

private:
	void SpawnEquipmentActor(const FAttachedSpawnInfo& AttachInfo);
	void DestroyEquipmentActor() const;

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> SpawnedActor = nullptr;

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
};
