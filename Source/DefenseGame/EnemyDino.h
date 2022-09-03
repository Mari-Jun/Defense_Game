// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyDino.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API AEnemyDino : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyDino();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void AttackHitStart() override;
	virtual void AttackHitEnd() override;

protected:
	UFUNCTION()
	virtual void OnBiteSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USphereComponent* BiteAttackSphereComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BiteAttackParticle;
};
