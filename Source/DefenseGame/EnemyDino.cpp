// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDino.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"

AEnemyDino::AEnemyDino()
{
	EnemyStatusData.DefaultSpeed = 650.f;
	EnemyStatusData.AttackSpeed = 500.f;
	EnemyStatusData.ReactionSpeed = 450.f;
	
	BiteAttackSphereComponent = CreateDefaultSubobject<USphereComponent>("BiteAttackSphereComponent");
	BiteAttackSphereComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "BiteAttackSocket");
	BiteAttackSphereComponent->SetCollisionProfileName("EnemyAttack");
}

void AEnemyDino::BeginPlay()
{
	Super::BeginPlay();

	BiteAttackSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyDino::OnBiteSphereOverlapEvent);
	BiteAttackSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyDino::AttackHitStart()
{
	BiteAttackSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (BiteAttackParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BiteAttackParticle, BiteAttackSphereComponent->GetComponentTransform());
	}
}

void AEnemyDino::AttackHitEnd()
{
	BiteAttackSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyDino::OnBiteSphereOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyDamage(OtherActor, EnemyStatusData.Attack);
	AttackHitEnd();
}
