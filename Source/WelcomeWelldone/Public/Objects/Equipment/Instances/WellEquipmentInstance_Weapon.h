// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WellEquipmentInstance.h"
#include "WellEquipmentInstance_Weapon.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellEquipmentInstance_Weapon : public UWellEquipmentInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animations)
	TObjectPtr<UAnimMontage> FireAnimMontage;

};
