// Copyright © 2025 bo6eR. All rights reserved.


#include "AbilitySystem/Attributes/WellAttributeSet_Damage.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


void UWellAttributeSet_Damage::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetDamage(FMath::Max(0, GetDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetHealAttribute())
	{
		SetHeal(FMath::Max(0, GetHeal()));
	}
}

void UWellAttributeSet_Damage::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Damage);
	DOREPLIFETIME(ThisClass, Heal);
}

void UWellAttributeSet_Damage::OnRep_Damage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Damage, OldValue)
}

void UWellAttributeSet_Damage::OnRep_Heal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Heal, OldValue);
}
