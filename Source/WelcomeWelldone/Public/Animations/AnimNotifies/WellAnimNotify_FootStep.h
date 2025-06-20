// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WellAnimNotify_FootStep.generated.h"


UENUM(BlueprintType)
enum EFootType
{
	Left,
	Right,
	
	MAX = 2 UMETA(Hidden)
};

UCLASS(meta=(DisplayName="AN_FootStep"))
class WELCOMEWELLDONE_API UWellAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

public:
	UWellAnimNotify_FootStep();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category=TraceData, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EFootType> FootType;

	UPROPERTY(Transient)
	float TraceDistance = 25.f;

private:
	FTraceDelegate TraceDelegate;
	void OnTraceDelegate(const FTraceHandle& Handle, FTraceDatum& Datum);
};
