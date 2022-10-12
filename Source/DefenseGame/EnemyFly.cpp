// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFly.h"
#include "Projectile.h"
#include "EnemyController.h"
#include "BaseCharacter.h"
#include "DefenseBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

void AEnemyFly::AttackHitStart()
{
	//BiteAttackSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (AttackProjectile != nullptr)
	{
		if (EnemyController->HasTarget())
		{
			FVector Location = GetMesh()->GetBoneLocation("jaw");
			FVector TargetLocation = EnemyController->GetAttackTargetLocation();

			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, TargetLocation);
			AProjectile* Spit = AProjectile::SpawnProjectile(AttackProjectile, FTransform{ Rotation, Location },
				this, CombatStatus.Attack, GetDamageTypeClass());
			Spit->SetActorScale3D(FVector(2.0f));
		}
	}
}

void AEnemyFly::KillObject()
{
	Super::KillObject();
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
}
