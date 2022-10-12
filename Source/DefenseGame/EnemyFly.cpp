// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFly.h"

#include "GameFramework/CharacterMovementComponent.h"

AEnemyFly::AEnemyFly()
{
	EnemyStatusData.DefaultSpeed = 400.f;
	EnemyStatusData.AttackSpeed = 350.f;
	EnemyStatusData.ReactionSpeed = 300.f;
}

void AEnemyFly::BeginPlay()
{
	Super::BeginPlay();

	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AEnemyFly::KillObject()
{
	Super::KillObject();
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}
