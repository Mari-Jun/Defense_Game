// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/Character.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("projectile mesh");
	SetRootComponent(ProjectileMesh);

	ProjectileMesh->SetSimulatePhysics(true);
	ProjectileMesh->SetEnableGravity(false);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	ProjectileMesh->SetMassOverrideInKg(NAME_None, 1.0f, true);
}

AProjectile* AProjectile::SpawnProjectile(TSubclassOf<AProjectile> ActorClass, FTransform SpawnTransform, ACharacter* Character, float Damage)
{
	AProjectile* Projectile = Cast<AProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(Character, ActorClass, SpawnTransform));
	if (Projectile != nullptr)
	{
		Projectile->SetOwnerCharacter(Character);
		Projectile->SetAttackDamage(Damage);
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

	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapEvent);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::FinishLifeTime()
{
	GetWorld()->DestroyActor(this);
}

void AProjectile::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Please set the owner character"));
	}
	else
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, AttackDamage, ProjectileMesh->GetComponentVelocity().GetSafeNormal(),
			SweepResult, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());
	}

	if (ImpactParticle != nullptr)
	{
		UParticleSystemComponent* SpawnedImpactParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorTransform());
		SpawnedImpactParticle->SetRelativeScale3D(FVector{ ImpactParticleScale });
	}

	GetWorld()->DestroyActor(this);
}

void AProjectile::AddImpulse(FVector WorldDirection)
{
	ProjectileMesh->AddImpulse(WorldDirection * ImpulseScale);
}

