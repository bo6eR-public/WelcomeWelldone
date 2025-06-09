// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WellEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS()
class WELCOMEWELLDONE_API UWellEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	void OnEquipped();
	void OnUneqipped();
	
};
