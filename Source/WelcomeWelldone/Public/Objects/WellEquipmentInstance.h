// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WellEquipmentInstance.generated.h"


UCLASS(Blueprintable)
class WELCOMEWELLDONE_API UWellEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	/* Should call from equipment component every time */
	virtual bool Initialize(AActor* SourceOwner);
	
	virtual void OnEquipped(const class UWellEquipmentProfile* OwningProfile);
	virtual void OnUneqipped();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	TWeakObjectPtr<ACharacter> OwningCharacter;
	
};
