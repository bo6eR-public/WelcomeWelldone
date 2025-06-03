// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/Characters/WellCharacter.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/WellAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"


AWellCharacter::AWellCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	
	AbilitySystemComponent = CreateDefaultSubobject<UWellAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	DefaultAttributeSet = CreateDefaultSubobject<UWellAttributeSet>(TEXT("AttributeSet"));
}

void AWellCharacter::AbilityInputPressed(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AWellCharacter::AbilityInputReleased(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AWellCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWellCharacter, StartUpData);
}

void AWellCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (!StartUpData.IsNull())
	{
		if (UObject* LoadedData = UAssetManager::GetStreamableManager().LoadSynchronous(StartUpData.ToSoftObjectPath()))
		{
			if (UWellCommonStartUpDataAsset* LoadedStartUpData = CastChecked<UWellCommonStartUpDataAsset>(LoadedData, ECastCheckedType::NullAllowed))
			{
				LoadedStartUpData->GiveToAbilitySystemComponent(AbilitySystemComponent);
			}
		}
	}
}

void AWellCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AWellCharacter::GetAbilitySystemComponent() const
{
	return GetWellAbilitySystemComponent();
}

UWellAbilitySystemComponent* AWellCharacter::GetWellAbilitySystemComponent() const
{
	return AbilitySystemComponent ? AbilitySystemComponent : nullptr;
}

UAttributeSet* AWellCharacter::GetAttributeSet() const
{
	return DefaultAttributeSet ? DefaultAttributeSet : nullptr;
}
