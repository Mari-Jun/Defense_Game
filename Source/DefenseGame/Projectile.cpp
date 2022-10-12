// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AProjectile* AProjectile::SpawnProjectile(TSubclassOf<AProjectile> ActorClass, FTransform SpawnTransform,
	ACharacter* Character, float Damage, TSubclassOf<UDamageType> DamageType)
{
	AProjectile* Projectile = Cast<AProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(Character, ActorClass, SpawnTransform));
	if (Projectile != nullptr)
	{
		Projectile->SetAttackDamage(Damage);
		Projectile->SetOwnerCharacter(Character);
		Projectile->SetDamageType(DamageType);
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
	return Projectile;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &AProjectile::FinishLifeTime, LifeTime);

	if (SpawnParticle != nullptr)
	{
		UParticleSystemComponent* SpawnedSpawnParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticle, GetActorTransform());
		SpawnedSpawnParticle->SetRelativeScale3D(FVector{ SpawnParticleScale });
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(Location);
}

void AProjectile::FinishLifeTime()
{
	GetWorld()->DestroyActor(this);
}

void AProjectile::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyPointDamage(OtherActor, AttackDamage, GetActorForwardVector(),
		SweepResult, OwnerCharacter->GetController(), this, DamageType);

	if (ImpactParticle != nullptr)
	{
		UParticleSystemComponent* SpawnedImpactParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorTransform());
		SpawnedImpactParticle->SetRelativeScale3D(FVector{ ImpactParticleScale });
	}

	GetWorld()->DestroyActor(this);
}
