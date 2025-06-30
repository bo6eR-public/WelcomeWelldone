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

	TSubclassOf<UGameplayAbility> GetAbilityClass() const;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup, meta=(AllowPrivateAccess=true))
	TSubclassOf<class AWellPickuppable> PickupItemDef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup, meta=(AllowPrivateAccess=true))
	TArray<FTransform> PickupTransforms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup, meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<UGameplayAbility>> PickUpAbilities;

	UPROPERTY(Replicated)
	TArray<AWellPickuppable*> SpawnedPickupActors;
};
