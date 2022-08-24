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

private:
	virtual void AbilityR() override;
	
protected:
	virtual void AttackLMBHit() override;
	virtual void AbilityQHit() override;
	virtual void AbilityEHit() override;
	virtual void AbilityRHit() override;
	virtual void AbilityRMBHit() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> AttackMagicBall;
};
