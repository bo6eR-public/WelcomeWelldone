// Copyright © 2025 bo6eR. All rights reserved.


#include "Characters/WellPlayerCharacter.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "CommomTypes/WellGameplayTags.h"
#include "Components/WellEnhancedInputComponent.h"
#include "DataAssets/Input/WellInputConfigDataAsset.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(WellPlayerCharacter)

AWellPlayerCharacter::AWellPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh(), TEXT("head"));
	Camera->SetRelativeTransform(FTransform
	(
	FRotator(0.f, 90.f, 0.f),
		FVector(5.f, 15.f, 0.f),
		FVector(0.5f, 0.5f, 0.5f)
	));
	Camera->bUsePawnControlRotation = true;

	GetMesh()->SetIsReplicated(true);
}

void AWellPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//
	// Don't forget to put your Enhanced input component in the project settings
	//
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!InputConfig) return;

	GetInputSubSystem()->ClearAllMappings();
	GetInputSubSystem()->AddMappingContext(InputConfig->MappingContext, 0);

	if (UWellEnhancedInputComponent* EnhancedInputComponent = Cast<UWellEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAbilityInputConfig(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
		EnhancedInputComponent->BindNativeInputAction(InputConfig, WellGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindNativeInputAction(InputConfig, WellGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
}

void AWellPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Subsystem name = %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
	}
}

void AWellPlayerCharacter::OverrideInputSettings(UWellInputConfigDataAsset* ApplyingInputConfig)
{
	GetInputSubSystem()->AddMappingContext(ApplyingInputConfig->MappingContext, 1);
	if (UWellEnhancedInputComponent* EnhancedInputComponent = Cast<UWellEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAbilityInputConfig(ApplyingInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased);
	}
}

void AWellPlayerCharacter::ResetInputSettings(UWellInputConfigDataAsset* RemovingInputConfig)
{
	GetInputSubSystem()->RemoveMappingContext(RemovingInputConfig->MappingContext);
	if (UWellEnhancedInputComponent* EnhancedInputComponent = Cast<UWellEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->UnBindAbilityInputConfig(RemovingInputConfig);
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

UEnhancedInputLocalPlayerSubsystem* AWellPlayerCharacter::GetInputSubSystem() const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;

	const AController* LocalController = GetController();
	if (LocalController && LocalController->IsLocalController())
	{
		const ULocalPlayer* LocalPlayer = Cast<APlayerController>(LocalController)->GetLocalPlayer();
		if (LocalPlayer != nullptr)
		{
			Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		}
	}
	checkf(Subsystem, TEXT("For some reason the input subsystem is not valid"));
	return Subsystem;
}
