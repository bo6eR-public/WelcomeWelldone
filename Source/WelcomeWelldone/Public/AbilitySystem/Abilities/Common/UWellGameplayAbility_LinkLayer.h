// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "UWellGameplayAbility_LinkLayer.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_LinkLayer : public UWellGameplayAbility
{
	GENERATED_BODY()

public:
	UWellGameplayAbility_LinkLayer();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=ActorInfo)
	UAnimInstance* GetAnimInstanceFromActorInfo() const;
	
};
