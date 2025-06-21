// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WellGameplayAbility.h"
#include "WellGameplayAbility_Pause.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellGameplayAbility_Pause : public UWellGameplayAbility
{
	GENERATED_BODY()

public:
	UWellGameplayAbility_Pause();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
private:
	class AWellHUD_Master* GetGameHUDFromPlayerController(const APlayerController* PlayerController) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data, meta=(AllowPrivateAccess=true))
	TSubclassOf<UUserWidget> PauseWidgetClass = nullptr;
	
};
