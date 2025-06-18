// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WellEquipmentInstance.h"
#include "WellEquipmentInstance_Weapon.generated.h"


UCLASS()
class WELCOMEWELLDONE_API UWellEquipmentInstance_Weapon : public UWellEquipmentInstance
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnEquipped(const UWellEquipmentProfile* OwningProfile) override;

public:
	UFUNCTION(BlueprintPure, Category=Ammo)
	FORCEINLINE int32 GetRemainingAmmo() { return AmmoRegister.X; }

	UFUNCTION(BlueprintPure, Category=Ammo)
	FORCEINLINE int32 GetMagazineAmmo() { return AmmoRegister.Y; }
	
	UFUNCTION(BlueprintPure, Category=Ammo)
	FORCEINLINE int32 GetTotalAmmo() { return AmmoRegister.Z; }

	UFUNCTION(BlueprintCallable, Category=Ammo)
	FORCEINLINE void SetRemainingAmmo(const int32 Value) { AmmoRegister.X = FMath::Max(0, Value); }

	UFUNCTION(BlueprintCallable, Category=Ammo)
	FORCEINLINE void SetTotalAmmo(const int32 Value) { AmmoRegister.Z = FMath::Max(0, Value); }

private:
	UPROPERTY(ReplicatedUsing=OnRep_AmmoRegister, EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category=Ammo)
	/* This is container for ammo of weapon:
	 * X coord means remaining ammo. Y coord means magazine ammo. Z coord means total ammo */
	FVector_NetQuantize100 AmmoRegister;

	UFUNCTION()
	void OnRep_AmmoRegister();

	/* Calls when instance have to notify locally UI about data changes */
	void BroadcastWeaponDataChanges();
};
