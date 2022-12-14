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

	SetStatusTable(TEXT("/Game/_Game/Enemys/Dino/DinoStatusTable"));
	SetBehaviorTree(TEXT("/Game/_Game/Enemys/Dino/BT_Dino"));
	SetAnimationInstanceClass(TEXT("/Game/_Game/Enemys/Dino/BP_DinoAnim"));

	SetIdleAnimSequence(TEXT("AnimSequence'/Game/_Game/Enemys/Dino/Animations/Dino_Idle.Dino_Idle'"));
	SetDeadAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/Dino/Animations/Dino_DeathMontage.Dino_DeathMontage'"));
	SetHitReactionFwdAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/Dino/Animations/Dino_HitFWDMontage.Dino_HitFWDMontage'"));
	SetHitReactionRightAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/Dino/Animations/Dino_HitRightMontage.Dino_HitRightMontage'"));
	SetHitReactionLeftAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/Dino/Animations/Dino_HitLeftMontage.Dino_HitLeftMontage'"));
	SetHitReactionBwdAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/Dino/Animations/Dino_HitBWDMontage.Dino_HitBWDMontage'"));

	AddNewAbility<USphereComponent>("DefaultAttack", 2.0f, 1.0f, 100'000, {
		"AnimMontage'/Game/_Game/Enemys/Dino/Animation/Dino_Attack_A_Montage.Dino_Attack_A_Montage'",
		"AnimMontage'/Game/_Game/Enemys/Dino/Animation/Dino_Attack_B_Montage.Dino_Attack_B_Montage'",
		});
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
	ApplyDamage(OtherActor, CombatStatus.Attack);
	AttackHitEnd();
}
