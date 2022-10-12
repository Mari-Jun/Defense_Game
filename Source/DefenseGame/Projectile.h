// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UShapeComponent;
class ACharacter;

UCLASS()
class DEFENSEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	static AProjectile* SpawnProjectile(TSubclassOf<AProjectile> ActorClass, FTransform SpawnTransform,
		ACharacter* Character, float Damage, TSubclassOf<UDamageType> DamageType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void FinishLifeTime();

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LifeTime = 5.0f;
	FTimerHandle LifeTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SpawnParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	float SpawnParticleScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Particle", meta = (AllowPrivateAccess = "true"))
	float ImpactParticleScale = 1.0f;

	ACharacter* OwnerCharacter;
	float AttackDamage = 0.f;
	TSubclassOf<UDamageType> DamageType;

public:
	void SetOwnerCharacter(ACharacter* OwnerChar) { OwnerCharacter = OwnerChar; }
	void SetMoveSpeed(float Speed) { MoveSpeed = Speed; }
	void SetAttackDamage(float Damage) { AttackDamage = Damage; }
	void SetDamageType(TSubclassOf<UDamageType> Type) { DamageType = Type; }
};
