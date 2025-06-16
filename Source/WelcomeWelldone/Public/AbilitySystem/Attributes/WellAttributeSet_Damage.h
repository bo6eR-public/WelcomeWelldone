// Copyright © 2025 bo6eR. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "WellAttributeSet_Damage.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class WELCOMEWELLDONE_API UWellAttributeSet_Damage : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Damage, Category=Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UWellAttributeSet_Damage, Damage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Heal, Category=Heal)
	FGameplayAttributeData Heal;
	ATTRIBUTE_ACCESSORS(UWellAttributeSet_Damage, Heal)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Heal(const FGameplayAttributeData& OldValue);
};
