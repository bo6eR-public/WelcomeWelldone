// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "WellCharacter.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWellCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UWellAbilitySystemComponent* GetWellAbilitySystemComponent() const;
	FORCEINLINE class UAttributeSet* GetAttributeSet() const;
	
protected:
	void AbilityInputPressed(FGameplayTag InInputTag);
	void AbilityInputReleased(FGameplayTag InInputTag);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAttributeSet_Core> DefaultAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellEquipmentComponent> EquipmentComponent;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category=StartUpData, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<class UWellCommonStartUpDataAsset> StartUpData;

};
