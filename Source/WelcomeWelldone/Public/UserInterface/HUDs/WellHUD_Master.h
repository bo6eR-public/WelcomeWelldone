// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "WellHUD_Master.generated.h"


UCLASS()
class WELCOMEWELLDONE_API AWellHUD_Master : public AHUD
{
	GENERATED_BODY()

public:
	virtual void InitializeHUD();
	virtual void UninitializeHUD();

	UFUNCTION(BlueprintCallable, Category=Layers)
	void PushToLayers(const TSubclassOf<UUserWidget>& LayerClass, bool bHiddenLastLayer = false);

	UFUNCTION(BlueprintCallable, Category=Layers)
	void PopFromLayers();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config, meta=(AllowPrivateAccess=true))
	TSubclassOf<UUserWidget> MasterLayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config, meta=(AllowPrivateAccess=true))
	bool bForceInitialize = false;

private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> ActivatedLayers;

	UPROPERTY()
	int CurrentZOrder = 0;
};
