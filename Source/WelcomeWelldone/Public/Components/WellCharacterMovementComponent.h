// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WellCharacterMovementComponent.generated.h"


USTRUCT()
struct FCharacterGroundInfo
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	FHitResult HitResult;

	UPROPERTY()
	float GroundDistance;
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WELCOMEWELLDONE_API UWellCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWellCharacterMovementComponent();
	
	/* Uses in the main animation blueprint */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Data)
	float TryGetGroundDistance() const;
};
