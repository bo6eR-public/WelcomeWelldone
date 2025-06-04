// Copyright © 2025 bo6eR. All rights reserved.


#include "Characters/WellPlayerCharacter.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Components/WellEnhancedInputComponent.h"
#include "DataAssets/Input/WellInputConfigDataAsset.h"


AWellPlayerCharacter::AWellPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AWellPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//
	// Don't forget to put your Enhanced input component in the project settings
	//
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!InputConfig) return;

	const ULocalPlayer* LocalPlayer = Cast<APlayerController>(GetController())->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	checkf(Subsystem, TEXT("For some reason the input subsystem is not valid"));

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputConfig->MappingContext, 0);

	if (UWellEnhancedInputComponent* EnhancedInputComponent = Cast<UWellEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAbilityInputConfig(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
		EnhancedInputComponent->BindNativeInputAction(InputConfig, WellGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindNativeInputAction(InputConfig, WellGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
}

void AWellPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AWellPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (MovementVector.X != 0.f || MovementVector.Y != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWellPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr) return;
	const FVector2D LookAxis = Value.Get<FVector2D>();
	
	if (LookAxis.X != 0.f)
	{
		AddControllerYawInput(LookAxis.X);
	}
	if (LookAxis.Y != 0.f)
	{
		AddControllerPitchInput(-LookAxis.Y);
	}
}
