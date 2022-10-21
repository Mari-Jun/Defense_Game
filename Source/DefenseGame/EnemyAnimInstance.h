// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UFUNCTION()
	virtual void AnimNotify_FinishDeath();
	UFUNCTION()
	virtual void AnimNotify_ReactionEnd();
	UFUNCTION()
	virtual void AnimNotify_AttackHitStart();
	UFUNCTION()
	virtual void AnimNotify_AttackHitEnd();
	UFUNCTION()
	virtual void AnimNotify_AttackEnd();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	AEnemy* Enemy = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 0.0f;
};
