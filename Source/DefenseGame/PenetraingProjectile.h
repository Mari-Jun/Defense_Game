// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PenetraingProjectile.generated.h"

/**
 * 
 */

UCLASS()
class DEFENSEGAME_API APenetraingProjectile : public AProjectile
{
	GENERATED_BODY()
	
	
protected:
	virtual void BeginPlay() override;

	virtual void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TickDamage(AActor* OtherActor);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DamageTickTime = 0.5f;
	TMap<AActor*, FTimerHandle> OverlapActors;
};
