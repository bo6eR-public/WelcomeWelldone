// Copyright © 2025 bo6eR. All rights reserved.


#include "Objects/Actors/WellSpawner.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "Objects/Actors/PickUp/WellPickuppable.h"


AWellSpawner::AWellSpawner()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
}

void AWellSpawner::GiveAbility(const TSubclassOf<UGameplayAbility>& Ability, AActor* TargetActor, const int32 Level)
{
	if (const auto TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor); TargetAbilitySystemComponent != nullptr)
	{
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.Level = Level;
		AbilitySpec.SourceObject = TargetActor;
		
		TargetAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UMaterialInterface* AWellSpawner::RequestSyncLoadMaterial() const
{
	if (DisplayMaterials.IsEmpty()) return nullptr;

	const int RandomIndex = FMath::RandHelper(DisplayMaterials.Num());
	
	TSoftObjectPtr<UMaterialInterface> Material = DisplayMaterials[RandomIndex];
	if (!Material.IsNull())
	{
		UMaterialInterface* LoadedMaterial = UAssetManager::GetStreamableManager().LoadSynchronous<UMaterialInterface>(Material.ToSoftObjectPath());
		return LoadedMaterial;
	}
	return nullptr;
}

TSubclassOf<UGameplayAbility> AWellSpawner::GetAbilityClass() const
{
	if (PickUpAbilities.IsEmpty()) return nullptr;

	const int RandomIndex = FMath::RandHelper(PickUpAbilities.Num());
	return PickUpAbilities[RandomIndex];
}

void AWellSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (!PickupTransforms.IsEmpty() && !PickUpAbilities.IsEmpty())
		{
			for (const FTransform Position : PickupTransforms)
			{
				const int RandIndex = FMath::RandHelper(PickUpAbilities.Num());
				AWellPickuppable* NewPickUp = GetWorld()->SpawnActor<AWellPickuppable>(PickupItemDef, Position.GetLocation(), Position.Rotator());
				if (NewPickUp != nullptr)
				{
					SpawnedPickupActors.Add(NewPickUp);
					NewPickUp->InitializePickuppable(this, PickUpAbilities[RandIndex]);
				}
			}
		}
	}
}

void AWellSpawner::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedPickupActors);
}
