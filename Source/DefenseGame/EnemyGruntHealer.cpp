// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGruntHealer.h"

#include "Kismet/GameplayStatics.h"

AEnemyGruntHealer::AEnemyGruntHealer()
{
	EnemyStatusData.DefaultSpeed = 400.f;
	EnemyStatusData.AttackSpeed = 0.f;
	EnemyStatusData.ReactionSpeed = 300.f;

	SetStatusTable(TEXT("/Game/_Game/Enemys/GruntHealer/GruntHealerStatusTable"));
	SetBehaviorTree(TEXT("/Game/_Game/Enemys/GruntHealer/BT_GruntHealer"));
	SetAnimationInstanceClass(TEXT("/Game/_Game/Enemys/GruntHealer/BP_GruntHealerAnim"));

	SetDeadAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/GruntHealer/Animations/GruntHealer_DeathMontage.GruntHealer_DeathMontage'"));
	SetHitReactionFwdAnimMontage(TEXT("AnimMontage'/Game/_Game/Enemys/GruntHealer/Animations/GruntHealer_Reaction_Fwd_Montage.GruntHealer_Reaction_Fwd_Montage'"));
}

void AEnemyGruntHealer::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyGruntHealer::KillObject()
{
	Super::KillObject();
}
