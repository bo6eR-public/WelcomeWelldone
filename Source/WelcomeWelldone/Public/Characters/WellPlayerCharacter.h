// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WellCharacter.h"
#include "WellPlayerCharacter.generated.h"


class UWellInputConfigDataAsset;

UCLASS()
class WELCOMEWELLDONE_API AWellPlayerCharacter : public AWellCharacter
{
	GENERATED_BODY()

public:
	AWellPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	void OverrideInputSettings(UWellInputConfigDataAsset* ApplyingInputConfig);
	void ResetInputSettings(UWellInputConfigDataAsset* RemovingInputConfig);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

private:
	//~ Begin of input handles
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void Optional1(const FInputActionValue& Value);
	void Optional2(const FInputActionValue& Value);
	void Optional3(const FInputActionValue& Value);
	//~ End of input handles

	class UEnhancedInputLocalPlayerSubsystem* GetInputSubSystem() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	TObjectPtr<UWellInputConfigDataAsset> InputConfig = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	TObjectPtr<class UCameraComponent> Camera;

};
