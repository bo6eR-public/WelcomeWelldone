// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WellEquipmentProfile.generated.h"


class UWellInputConfigDataAsset;
class UWellEquipmentInstance;

UCLASS(Blueprintable, Const, Abstract)
class WELCOMEWELLDONE_API UWellEquipmentProfile : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE [[nodiscard]] UWellInputConfigDataAsset* GetInputConfig() const { return OverridingInputConfig; }
	FORCEINLINE [[nodiscard]] TSubclassOf<UAnimInstance> GetAnimationLayer() const { return AnimationLayer; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Instance)
	TSubclassOf<UWellEquipmentInstance> EquipmentInstance;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWellInputConfigDataAsset> OverridingInputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animation, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UAnimInstance> AnimationLayer;

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	
};
