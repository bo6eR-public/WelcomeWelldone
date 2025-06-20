// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WellPhysMat_Surface.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellPhysMat_Surface : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Sound)
	USoundBase* SurfaceSound = nullptr;
};
