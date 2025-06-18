// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommomTypes/WellCommonTypes.h"
#include "Blueprint/UserWidget.h"
#include "WellUserWidget_OverlayFragment.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellUserWidget_OverlayFragment : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, Category=Attribute)
	void OnAttributeChanged(FGameplayTag Tag, const FGameplayMessage_AttributeChanged& Message);
	
};
