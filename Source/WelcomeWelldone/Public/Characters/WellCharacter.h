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
	AWellCharacter();
	
#pragma region AbilitySystem
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UWellAbilitySystemComponent* GetWellAbilitySystemComponent() const;
	FORCEINLINE class UAttributeSet* GetAttributeSet() const;
#pragma endregion 
	
protected:
	void AbilityInputPressed(FGameplayTag InInputTag);
	void AbilityInputReleased(FGameplayTag InInputTag);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAttributeSet> DefaultAttributeSet;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category=StartUpData, meta=(AllowPrivateAccess="true"))
	TSoftObjectPtr<class UWellCommonStartUpDataAsset> StartUpData;

};
