// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WellPickuppable.generated.h"

UCLASS()
class WELCOMEWELLDONE_API AWellPickuppable : public AActor
{
	GENERATED_BODY()

public:
	AWellPickuppable();
	virtual void Tick(float DeltaTime) override;

	void InitializePickuppable(class AWellSpawner* NewSpawnerController, TSubclassOf<class UGameplayAbility> Ability);
	UFUNCTION(NetMulticast, Reliable)
	void SetVisibility(bool Visibility);
	UFUNCTION()
	void StartSpawnDelay();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class USphereComponent> CollisionVolume = nullptr;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<UStaticMeshComponent> DisplayMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Settings, meta=(ClampMin="0.0", UIMin="0.0"))
	float RotationSpeed = 10.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category=Ability)
	TSubclassOf<class UGameplayAbility> PickuUpAbility = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Pickup)
	float SpawnDelay = 10.f;

private:
	UPROPERTY(Replicated)
	TObjectPtr<AWellSpawner> Spawner = nullptr;

	UPROPERTY(Replicated)
	FTimerHandle SpawnDelayTimerHandle;

private:
	UFUNCTION()
	void OnCollisionVolumeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
};
