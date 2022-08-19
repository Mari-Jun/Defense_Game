// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterCaster.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API ACharacterCaster : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ACharacterCaster();
	

public:
	virtual void AttackHit() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> AttackMagicBall;
};
