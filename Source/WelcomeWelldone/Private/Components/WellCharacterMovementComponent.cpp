// Copyright © 2025 bo6eR. All rights reserved.


#include "Components/WellCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


UWellCharacterMovementComponent::UWellCharacterMovementComponent()
{
	bUseControllerDesiredRotation = true;
	RotationRate = FRotator(0.f, 7200.f, 0.f);
}

float UWellCharacterMovementComponent::TryGetGroundDistance() const
{
	if (!CharacterOwner || MovementMode == MOVE_Walking) { return 0.f; }
	
	const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
	check(CapsuleComp);

	const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
	const ECollisionChannel CollisionChannel = UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn;
	const FVector TraceStart(GetActorLocation());
	const FVector TraceEnd(TraceStart.X, TraceStart.Y, TraceStart.Z - CapsuleHalfHeight * 2.5f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CharacterOwner);
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam);
	
	return HitResult.bBlockingHit ? FMath::Max(0.f, HitResult.Distance - CapsuleHalfHeight) : 0.f;
}
