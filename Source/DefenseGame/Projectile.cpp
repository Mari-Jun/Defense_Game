// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

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

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &AProjectile::FinishLifeTime, LifeTime);

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
	GetWorld()->DestroyActor(this);
}

void AProjectile::AddImpulse(FVector WorldDirection)
{
	ProjectileMesh->AddImpulse(WorldDirection * ImpulseScale);
}

