// Copyright © 2025 bo6eR. All rights reserved.


#include "Animations/AnimNotifies/WellAnimNotify_FootStep.h"
#include "Animations/Meshes/PhysicMaterials/WellPhysMat_Surface.h"
#include "Kismet/GameplayStatics.h"

UWellAnimNotify_FootStep::UWellAnimNotify_FootStep()
{
	TraceDelegate.BindUObject(this, &ThisClass::OnTraceDelegate);
}

FString UWellAnimNotify_FootStep::GetNotifyName_Implementation() const
{
	return FString(TEXT("AN_Step"));
}

void UWellAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	check(MeshComp);

	const FName SocketName = FootType == Left ? FName("foot_l") : FName("foot_r");
	const FVector StartLocation = MeshComp->GetSocketLocation(SocketName);
	const FVector EndLocation = StartLocation - FVector::UpVector * TraceDistance;

	if (UWorld* World = MeshComp->GetWorld())
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MeshComp->GetOwner());
		QueryParams.bReturnPhysicalMaterial = true;
		
		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, StartLocation, EndLocation, ECC_Visibility,
			QueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	}
}

void UWellAnimNotify_FootStep::OnTraceDelegate(const FTraceHandle& Handle, FTraceDatum& Datum)
{
	if (Handle.IsValid() && Datum.OutHits.Num())
	{
		const FHitResult& OutHit = Datum.OutHits[0];
		if (OutHit.bBlockingHit && OutHit.PhysMaterial.IsValid())
		{
			UWellPhysMat_Surface* PhysMat = Cast<UWellPhysMat_Surface>(OutHit.PhysMaterial.Get());
			if (PhysMat && PhysMat->SurfaceSound)
			{
				UGameplayStatics::PlaySoundAtLocation(Datum.PhysWorld.Get(), PhysMat->SurfaceSound, OutHit.Location);
			}
		}
	}
}
