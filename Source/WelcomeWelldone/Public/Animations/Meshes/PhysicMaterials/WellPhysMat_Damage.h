// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WellPhysMat_Damage.generated.h"

/**
 * The physic material for parts of character mesh physic asset
 */
UCLASS()
class WELCOMEWELLDONE_API UWellPhysMat_Damage : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UWellPhysMat_Damage(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Damage, meta=(ClampMin="0.1", ClampMax="3.0", UIMin="0.1"))
	float DamageMultiplier;
};
