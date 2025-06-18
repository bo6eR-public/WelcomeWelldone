// Copyright © 2025 bo6eR. All rights reserved.


#include "WelcomeWelldone/Public/Characters/WellCharacter.h"
#include "AbilitySystem/AbilitySystemComponents/WellAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/WellAttributeSet_Core.h"
#include "AbilitySystem/Attributes/WellAttributeSet_Damage.h"
#include "Components/CapsuleComponent.h"
#include "Components/WellCharacterMovementComponent.h"
#include "Components/WellEquipmentComponent.h"
#include "DataAssets/StartUp/WellCommonStartUpDataAsset.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WellCharacter)

AWellCharacter::AWellCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UWellCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetCapsuleRadius(48.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UWellAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CoreAttributeSet = CreateDefaultSubobject<UWellAttributeSet_Core>(TEXT("CoreAttributeSet"));
	DamageAttributeSet = CreateDefaultSubobject<UWellAttributeSet_Damage>(TEXT("DamageAttributeSet"));

	//~ Lambda function for infinity gameplay effects
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CoreAttributeSet->GetMaxMovementSpeedAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data)
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	});

	EquipmentComponent = CreateDefaultSubobject<UWellEquipmentComponent>(TEXT("EquipmentComponent"));
	bReplicateUsingRegisteredSubObjectList = true;
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
				LoadedStartUpData->GiveToAbilitySystemComponent(AbilitySystemComponent, this);
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
