// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatHelperInterface.generated.h"

USTRUCT(BlueprintType)
struct FDefaultCombatStatus
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentShield = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxShield = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Defense = 50.f;
};

USTRUCT(BlueprintType)
struct FAttackCombatStatus : public FDefaultCombatStatus
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Critical = 0.f;
};

USTRUCT(BlueprintType)
struct FReactionCombatStatus : public FAttackCombatStatus
{
	GENERATED_BODY()

	float CurrentReactionValue = 0.f;
	/** °æÁ÷Ä¡ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReactionValue = 50.f;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeHPDelegate, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeShieldDelegate, float, CurrentShield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeReactionDelegate, float, CurrentReaction, float, MaxReaction);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatHelperInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEFENSEGAME_API ICombatHelperInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float CalcDamage(float Attack, float Critical, float OwnerDefense);

};
