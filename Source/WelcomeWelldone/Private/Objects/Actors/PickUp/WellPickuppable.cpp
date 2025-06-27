// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/Actors/PickUp/WellPickuppable.h"
#include "Objects/Actors/WellSpawner.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


AWellPickuppable::AWellPickuppable()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionVolumeOverlapped);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMeshComponent"));
	DisplayMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AWellPickuppable::OnCollisionVolumeOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor != nullptr && OtherActor->HasAuthority())
	{
		if (OtherActor->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			Spawner->GiveAbility(PickuUpAbility, OtherActor);
			SetVisibility(false);
			StartSpawnDelay();
		}
	}
}

void AWellPickuppable::OnRep_Spawner()
{
	if (UMaterialInterface* DisplayedMaterial = Spawner->RequestSyncLoadMaterial())
	{
		DisplayMesh->SetMaterial(0, DisplayedMaterial);
	}
}

void AWellPickuppable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DisplayMesh->AddRelativeRotation(FRotator(0.0f, GetWorld()->GetDeltaSeconds() * RotationSpeed, 0.0f));
}

void AWellPickuppable::InitializePickuppable(AWellSpawner* NewSpawnerController, TSubclassOf<UGameplayAbility> Ability)
{
	Spawner = NewSpawnerController;
	PickuUpAbility = Ability;
}

void AWellPickuppable::SetVisibility_Implementation(const bool Visibility)
{
	CollisionVolume->SetVisibility(Visibility);
	DisplayMesh->SetVisibility(Visibility);
}

void AWellPickuppable::StartSpawnDelay_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		SetVisibility(true);
		GetWorld()->GetTimerManager().ClearTimer(SpawnDelayTimerHandle);
		if (HasAuthority())
		{
			PickuUpAbility = Spawner->GetAbilityClass();
		}
	}), SpawnDelay, false);
}

void AWellPickuppable::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Spawner);
	DOREPLIFETIME(ThisClass, DisplayMesh);
}
