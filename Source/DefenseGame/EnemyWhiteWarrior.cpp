// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWhiteWarrior.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemyWhiteWarrior::AEnemyWhiteWarrior()
{
	EnemyStatusData.DefaultSpeed = 600.f;
	EnemyStatusData.AttackSpeed = 550.f;
	EnemyStatusData.ReactionSpeed = 400.f;

	AttackBoxComponent = CreateDefaultSubobject<UBoxComponent>("AttackBoxComponent");
	AttackBoxComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "AttackSocket");
	AttackBoxComponent->SetCollisionProfileName("EnemyAttack");

	AddNewAbility<USphereComponent>("DefaultAttack", 2.0f, 1.0f, 100'000, {
		"AnimMontage'/Game/_Game/Enemys/WhiteWarrior/Animations/WhiteWarrior_Attack_A_Montage.WhiteWarrior_Attack_A_Montage'",
		"AnimMontage'/Game/_Game/Enemys/WhiteWarrior/Animations/WhiteWarrior_Attack_B_Montage.WhiteWarrior_Attack_B_Montage'",
		"AnimMontage'/Game/_Game/Enemys/WhiteWarrior/Animations/WhiteWarrior_Attack_D_Montage.WhiteWarrior_Attack_D_Montage'"
		});
	AddNewAbility<UBoxComponent>("Strike", 10.0f, 1.5f, 100, {
		"AnimMontage'/Game/_Game/Enemys/WhiteWarrior/Animations/WhiteWarrior_Strike_Montage.WhiteWarrior_Strike_Montage'"
		});
}

void AEnemyWhiteWarrior::BeginPlay()
{
	Super::BeginPlay();

	AttackBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyWhiteWarrior::OnAttackBoxOverlapEvent);
	AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyWhiteWarrior::AttackHitStart()
{
	AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyWhiteWarrior::AttackHitEnd()
{
	AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyWhiteWarrior::KillObject()
{
	Super::KillObject();
	if (DeathParticle != nullptr)
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, GetActorTransform());
		Particle->SetRelativeScale3D(GetMesh()->GetRelativeScale3D());
	}
}

void AEnemyWhiteWarrior::OnAttackBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyDamage(OtherActor, CombatStatus.Attack);
	if (AttackParticle != nullptr)
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, OtherActor->GetActorTransform());
		Particle->SetRelativeScale3D(GetMesh()->GetRelativeScale3D());
	}
	AttackHitEnd();
}
