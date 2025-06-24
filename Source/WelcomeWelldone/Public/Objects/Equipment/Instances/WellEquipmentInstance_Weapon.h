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
	FORCEINLINE int32 GetRemainingAmmo() { return RemainingAmmo; }

	UFUNCTION(BlueprintPure, Category=Ammo)
	FORCEINLINE int32 GetMagazineAmmo() { return MagazineAmmo; }
	
	UFUNCTION(BlueprintPure, Category=Ammo)
	FORCEINLINE int32 GetTotalAmmo() { return TotalAmmo; }

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category=Ammo)
	void SetRemainingAmmo(const int32 Value);

	UFUNCTION(BlueprintCallable, Category=Ammo)
	void SetTotalAmmo(const int32 Value);

private:
	UPROPERTY(ReplicatedUsing=OnRep_AmmoRegister, EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category=Ammo)
	int32 RemainingAmmo;
	UPROPERTY(ReplicatedUsing=OnRep_AmmoRegister, EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category=Ammo)
	int32 MagazineAmmo;
	UPROPERTY(ReplicatedUsing=OnRep_AmmoRegister, EditDefaultsOnly, meta=(AllowPrivateAccess="true"), Category=Ammo)
	int32 TotalAmmo;

	UFUNCTION()
	void OnRep_AmmoRegister();

	/* Calls when instance have to notify locally UI about data changes */
	void BroadcastWeaponDataChanges();
};
