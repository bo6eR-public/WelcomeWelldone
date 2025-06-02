// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/Characters/WellCharacter.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/WellAttributeSet.h"
#include "Camera/CameraComponent.h"


AWellCharacter::AWellCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	
	AbilitySystemComponent = CreateDefaultSubobject<UWellAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	DefaultAttributeSet = CreateDefaultSubobject<UWellAttributeSet>(TEXT("AttributeSet"));
}

void AWellCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWellCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AWellCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent ? AbilitySystemComponent : nullptr;
}

UAttributeSet* AWellCharacter::GetAttributeSet() const
{
	return DefaultAttributeSet ? DefaultAttributeSet : nullptr;
}
