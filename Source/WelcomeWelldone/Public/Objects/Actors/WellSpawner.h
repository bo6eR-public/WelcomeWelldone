// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WellSpawner.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellSpawner : public AActor
{
	GENERATED_BODY()

public:
	AWellSpawner();
	
	void GiveAbility(const TSubclassOf<class UGameplayAbility>& Ability, AActor* TargetActor, int32 Level = 0);
	UMaterialInterface* RequestSyncLoadMaterial() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Materials, meta=(AllowPrivateAccess=true))
	TArray<FTransform> PickupTransforms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Materials, meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<class AWellPickuppable>> PickupItemsClasses;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Materials, meta=(AllowPrivateAccess=true))
	TArray<TSoftObjectPtr<UMaterialInterface>> DisplayMaterials;

	UPROPERTY(Replicated)
	TArray<AWellPickuppable*> SpawnedPickupActors;
};
