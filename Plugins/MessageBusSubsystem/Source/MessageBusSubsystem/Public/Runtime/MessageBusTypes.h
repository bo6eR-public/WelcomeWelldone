// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "MessageBusTypes.generated.h"


UENUM(BlueprintType)
enum class EGameplayMessageMatch : uint8
{
	ExactMatch,
	PartialMatch
};


template<typename FMessageStructType>
struct FGameplayMessageListenerParams
{
	EGameplayMessageMatch MatchType = EGameplayMessageMatch::ExactMatch;
	
	TFunction<void(FGameplayTag, const FMessageStructType&)> OnMessageReceivedCallback;
};
