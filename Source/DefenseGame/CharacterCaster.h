// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterCaster.generated.h"

class AProjectile;
class ADefenseObject;
class AEnemy;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API ACharacterCaster : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ACharacterCaster();

protected:
	virtual void BeginPlay() override;

private:
	virtual void AbilityR(int32 AbilityIndex) override;
	
protected:
	virtual void AttackLMBHit() override;
	virtual void AbilityQHit() override;
	virtual void AbilityEHit() override;
	virtual void AbilityRHit() override;
	virtual void AbilityRMBHit() override;

private:
	void FinishAbilityQShield();
	UFUNCTION()
	void OnMagicShieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMagicShieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void AbilityQTickDamage(AEnemy* Enemy);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> AttackMagicBall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AbilityQMagicShield;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AbilityQDuration = 3.0f;
	FTimerHandle AbilityQDurationTimerHandle;
	TMap<AActor*, FTimerHandle> AbilityQOverlapActors;

};
