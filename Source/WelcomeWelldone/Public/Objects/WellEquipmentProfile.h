// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "WellEquipmentProfile.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellEquipmentProfile : public UObject
{
	GENERATED_BODY()

public:
	class UWellEquipmentInstance* GetDefaultInstance() const { return EquipmentInstance.GetDefaultObject(); }
	[[nodiscard]] UWellCommonStartUpDataAsset* GetApplyingData() const { return ApplyingData; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Instance, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UWellEquipmentInstance> EquipmentInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Ability, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWellCommonStartUpDataAsset> ApplyingData;
	
};
