// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CriticalDamageType.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UCriticalDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	float Critical = 0.0f;
};
