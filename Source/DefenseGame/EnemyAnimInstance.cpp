// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn != nullptr)
	{
		Enemy = Cast<AEnemy>(OwnerPawn);
		if (Enemy == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast TryGetPawnOwner() to AEnemy"));
		}
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Enemy != nullptr)
	{
		Speed = Enemy->GetVelocity().Length();
	}
}
