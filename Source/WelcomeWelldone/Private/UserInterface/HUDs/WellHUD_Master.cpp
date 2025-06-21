// Copyright © 2025 bo6eR. All rights reserved.


#include "UserInterface/HUDs/WellHUD_Master.h"
#include "Blueprint/UserWidget.h"


void AWellHUD_Master::InitializeHUD()
{
	const auto OwningController = GetOwningPlayerController();
	check(OwningController);
	
	if (MasterLayer != nullptr)
	{
		UUserWidget* NewLayer = CreateWidget(OwningController, MasterLayer);
		NewLayer->AddToViewport(++CurrentZOrder);
			
		ActivatedLayers.Emplace(NewLayer);
	}
}

void AWellHUD_Master::UninitializeHUD()
{
	for (const auto LayerItem : ActivatedLayers)
	{
		if (LayerItem != nullptr)
		{
			LayerItem->RemoveFromParent();
		}
	}
	ActivatedLayers.Empty();
}

void AWellHUD_Master::PushToLayers(const TSubclassOf<UUserWidget>& LayerClass, bool bHiddenLastLayer)
{
	if (LayerClass != nullptr)
	{
		if (bHiddenLastLayer)
		{
			ActivatedLayers.Last()->SetVisibility(ESlateVisibility::Hidden);
		}
		UUserWidget* NewLayer = CreateWidget(GetOwningPlayerController(), LayerClass);
		NewLayer->AddToViewport(++CurrentZOrder);
		
		ActivatedLayers.Emplace(NewLayer);
	}
}

void AWellHUD_Master::PopFromLayers()
{
	if (UUserWidget* LastLayer = ActivatedLayers.Pop())
	{
		LastLayer->RemoveFromParent();
		CurrentZOrder = FMath::Max(0, CurrentZOrder-1);
		if (ActivatedLayers.Num() && !ActivatedLayers.Last()->IsVisible())
		{
			ActivatedLayers.Last()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AWellHUD_Master::BeginPlay()
{
	Super::BeginPlay();
	
	if (bForceInitialize)
	{
		/* This one is for main menu, for example */
		InitializeHUD();
	}
}
