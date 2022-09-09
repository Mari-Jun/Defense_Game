// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class DEFENSEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	static AProjectile* SpawnProjectile(TSubclassOf<AProjectile> ActorClass, FTransform SpawnTransform, ACharacter* Character, float Damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual void FinishLifeTime();

protected:
	UFUNCTION()
	virtual void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void AddImpulse(FVector WorldDirection);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LifeTime = 5.0f;
	FTimerHandle LifeTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ImpulseScale = 5000.f;

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

public:
	UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
	void SetOwnerCharacter(ACharacter* Character) { OwnerCharacter = Character; }
	void SetAttackDamage(float Damage) { AttackDamage = Damage; }
};
