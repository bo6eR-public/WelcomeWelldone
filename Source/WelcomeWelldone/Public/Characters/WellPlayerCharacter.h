// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WellCharacter.h"
#include "WellPlayerCharacter.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellPlayerCharacter : public AWellCharacter
{
	GENERATED_BODY()

public:
	AWellPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;

private:
	void Move(const FInputActionValue& Value); // input handle
	void Look(const FInputActionValue& Value); // input handle

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UWellInputConfigDataAsset* InputConfig = nullptr;

};
