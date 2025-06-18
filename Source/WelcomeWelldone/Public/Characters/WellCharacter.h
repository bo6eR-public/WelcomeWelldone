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
	
	FORCEINLINE class UWellAttributeSet_Core* GetBaseAttributeSet() const { return CoreAttributeSet; }
	FORCEINLINE class UWellAttributeSet_Damage* GetDamageAttributeSet() const { return DamageAttributeSet; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category=Equipment)
	FORCEINLINE class UWellEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	
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
	TObjectPtr<class UWellAttributeSet_Core> CoreAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAttributeSet_Damage> DamageAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellEquipmentComponent> EquipmentComponent;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category=StartUpData, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<class UWellCommonStartUpDataAsset> StartUpData;

};
