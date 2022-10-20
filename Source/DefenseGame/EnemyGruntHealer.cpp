// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGruntHealer.h"

#include "Kismet/GameplayStatics.h"

AEnemyGruntHealer::AEnemyGruntHealer()
{
	EnemyStatusData.DefaultSpeed = 400.f;
	EnemyStatusData.AttackSpeed = 0.f;
	EnemyStatusData.ReactionSpeed = 300.f;
}

void AEnemyGruntHealer::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyGruntHealer::KillObject()
{
	Super::KillObject();
}
