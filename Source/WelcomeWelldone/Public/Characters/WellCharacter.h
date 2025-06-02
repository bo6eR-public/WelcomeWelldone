// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "WellCharacter.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWellCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE class UAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UWellAttributeSet> DefaultAttributeSet;
#pragma endregion 

};
