// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CommomTypes/WellCommonTypes.h"
#include "WellEquipmentInstance.generated.h"


UCLASS(Blueprintable)
class WELCOMEWELLDONE_API UWellEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	/* Should call from equipment component every time */
	virtual bool Initialize(AActor* SourceOwner);
	virtual UWorld* GetWorld() const override;
	
	virtual bool OnEquipped(const class UWellEquipmentProfile* OwningProfile);
	virtual void OnUneqipped();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	FAttachedSpawnInfo AttachedActorInfo;

private:
	void SpawnEquipmentActor(const FAttachedSpawnInfo& AttachInfo);
	bool DestroyEquipmentActor() const;

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY(Replicated)
	TObjectPtr<AActor> SpawnedActor = nullptr;
};
