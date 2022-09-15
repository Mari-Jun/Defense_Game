// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatHelperInterface.h"

// Add default functionality here for any ICombatHelperInterface functions that are not pure virtual.

float ICombatHelperInterface::CalcDamage(float Attack, float Critical, float OwnerDefense)
{
	float DamageAmount = Attack;
	float DecresePercent = 0.0f;

	if (OwnerDefense <= 100.f)
	{
		float Alpha = OwnerDefense / 100.f;
		DecresePercent = FMath::Lerp<float>(0.0f, 0.5f, Alpha);
	}
	else if (OwnerDefense > 100.f && OwnerDefense <= 200.f)
	{
		float Alpha = (OwnerDefense - 100.f) / 100.f;
		DecresePercent = FMath::Lerp<float>(0.5f, 0.8f, Alpha);
	}
	else
	{
		float Alpha = (OwnerDefense - 200.f) / 800.f;
		DecresePercent = FMath::Lerp<float>(0.8f, 0.95f, Alpha);
	}

	DamageAmount -= DamageAmount * DecresePercent;

	if(FMath::IsNearlyZero(Critical) == false)
	{
		float RandCritical = FMath::FRandRange(0.0f, 1.0f);
		if (RandCritical <= Critical)
			DamageAmount *= 2.0f;
	}

	return DamageAmount;
}
