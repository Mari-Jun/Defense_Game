// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCaster.h"
#include "Projectile.h"
#include "Enemy.h"
#include "PenetraingProjectile.h"

#include "Animation/BlendSpace1D.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACharacterCaster::ACharacterCaster()
{
	USkeletalMesh* caster_skeletal_mesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr,
		TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Skins/Tier_1_5/Pandapack/Meshes/Phase_Pandapack_GDC.Phase_Pandapack_GDC'")));
	GetMesh()->SetSkeletalMesh(caster_skeletal_mesh);

	AbilityQMagicShield = CreateDefaultSubobject<UStaticMeshComponent>("Ability Q Magic Shield");
	AbilityQMagicShield->SetupAttachment(GetRootComponent());
	AbilityQMagicShield->SetCollisionProfileName("NoCollision");

	AbilityRMagicCylinder = CreateDefaultSubobject<UStaticMeshComponent>("Ability R Magic Cylinder");
	AbilityRMagicCylinder->SetupAttachment(GetRootComponent());
	AbilityRMagicCylinder->SetCollisionProfileName("NoCollision");

	CharacterAnimationData.IdleAnimSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Idle.Caster_Idle'")));
	CharacterAnimationData.TurnLeft90Sequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_TurnLeft_90.Caster_TurnLeft_90'")));
	CharacterAnimationData.TurnRight90Sequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_TurnRight_90.Caster_TurnRight_90'")));
	CharacterAnimationData.TurnRotationCurve = Cast<UCurveFloat>(StaticLoadObject(UCurveFloat::StaticClass(), nullptr,
		TEXT("CurveFloat'/Game/_Game/Characters/Caster/Curve/Caster_TurnRotation_Curve.Caster_TurnRotation_Curve'")));

	CharacterAnimationData.JogStartBlendSpace = Cast<UBlendSpace1D>(StaticLoadObject(UBlendSpace1D::StaticClass(), nullptr,
		TEXT("BlendSpace1D'/Game/_Game/Characters/Caster/Animations/BS_Caster_Jog_Start.BS_Caster_Jog_Start'")));
	CharacterAnimationData.JogBlendSpace = Cast<UBlendSpace1D>(StaticLoadObject(UBlendSpace1D::StaticClass(), nullptr,
		TEXT("BlendSpace1D'/Game/_Game/Characters/Caster/Animations/BS_Caster_Jog.BS_Caster_Jog'")));
	CharacterAnimationData.JogStopBlendSpace = Cast<UBlendSpace1D>(StaticLoadObject(UBlendSpace1D::StaticClass(), nullptr,
		TEXT("BlendSpace1D'/Game/_Game/Characters/Caster/Animations/BS_Caster_Jop_Stop.BS_Caster_Jop_Stop'")));

	CharacterAnimationData.JumpStartSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Jump_Start.Caster_Jump_Start'")));
	CharacterAnimationData.JumpApexSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Jump_Apex.Caster_Jump_Apex'")));
	CharacterAnimationData.JumpLandSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Jump_Land.Caster_Jump_Land'")));
	CharacterAnimationData.JumpRecoverySequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Jump_Recovery_Additive.Caster_Jump_Recovery_Additive'")));

	CharacterAnimationData.AttackAnimMontange.Add(
		Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
			TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Attack_A_Montage.Caster_Attack_A_Montage'")))
	);
	CharacterAnimationData.AttackAnimMontange.Add(
		Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
			TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Attack_B_Montage.Caster_Attack_B_Montage'")))
	);
	CharacterAnimationData.AttackAnimMontange.Add(
		Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
			TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Attack_C_Montage.Caster_Attack_C_Montage'")))
	);
	CharacterAnimationData.AttackAnimMontange.Add(
		Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
			TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Attack_D_Montage.Caster_Attack_D_Montage'")))
	);

	CharacterAnimationData.AbilityQMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
		TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Ability_Q_Montage.Caster_Ability_Q_Montage'")));
	CharacterAnimationData.AbilityEMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
		TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Ability_E_Montage.Caster_Ability_E_Montage'")));
	CharacterAnimationData.AbilityRMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
		TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Ability_R_Montage.Caster_Ability_R_Montage'")));

	CharacterAnimationData.DeathMontage = Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
		TEXT("AnimMontage'/Game/_Game/Characters/Caster/Animations/Caster_Death_Montage.Caster_Death_Montage'")));
}

void ACharacterCaster::BeginPlay()
{
	Super::BeginPlay();

	AbilityQMagicShield->SetVisibility(false);
	AbilityQMagicShield->OnComponentBeginOverlap.AddDynamic(this, &ACharacterCaster::OnMagicShieldBeginOverlap);
	AbilityQMagicShield->OnComponentEndOverlap.AddDynamic(this, &ACharacterCaster::OnMagicShieldEndOverlap);

	AbilityRMagicCylinder->SetVisibility(false);
	AbilityRMagicCylinder->OnComponentBeginOverlap.AddDynamic(this, &ACharacterCaster::OnAbilityRBeginOverlap);
	AbilityRMagicCylinder->OnComponentEndOverlap.AddDynamic(this, &ACharacterCaster::OnAbilityREndOverlap);
}

void ACharacterCaster::AbilityR(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityRMontage != nullptr)
	{
		if (AttackState == EAttackState::ENone && CheckAbilityCooldown(AbilityIndex))
		{
			PlayAnimMontage(CharacterAnimationData.AbilityRMontage);
			AttackState = EAttackState::EAbilityR;
			bIsAttackFull = true;

			StartAbilityCooldown(AbilityIndex);
		}
		else if (AttackState == EAttackState::EAbilityR)
		{
			float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(CharacterStatusData.AbilityTimerHandle[AbilityIndex]);
			if (ElapsedTime > 0.5f)
			{
				StopAnimMontage(CharacterAnimationData.AbilityRMontage);
				AttackEnd();
			}
		}
	}
}

void ACharacterCaster::AttackLMBHit()
{
	if (GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		FVector2D ViewportMidPos = ViewportSize * 0.5f;
		FVector CrosshairWorldPosition, CrosshairWorldDirection;

		if (UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
			ViewportMidPos, CrosshairWorldPosition, CrosshairWorldDirection))
		{
			if (AttackMagicBall != nullptr)
			{
				//애니메이션 커브를 통해서 왼쪽손에서의 공격인지, 오른쪽손에서의 공격인지 확인한다.
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					float AttackingHandValue = AnimInstance->GetCurveValue("AttackingHand");

					FVector SpawnPoint = (AttackingHandValue > 0.0f) ? GetMesh()->GetBoneLocation("hand_l") : GetMesh()->GetBoneLocation("hand_r");

					AProjectile* SpawnBall = AProjectile::SpawnProjectile(AttackMagicBall, { FRotator{}, SpawnPoint },
						this, CombatStatus.Attack, GetDamageTypeClass());

					const FVector Start{ CrosshairWorldPosition };
					const FVector End{ Start + CrosshairWorldDirection * 50'000 };

					FVector ImpulseWorldDirection = CrosshairWorldDirection;

					FHitResult HitResult;
					if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
					{
						ImpulseWorldDirection = HitResult.ImpactPoint - SpawnPoint;
						ImpulseWorldDirection.Normalize();
					}

					FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, SpawnPoint + ImpulseWorldDirection);
					SpawnBall->SetActorRotation(NewRotation);
				}
			}
		}
	}
}

void ACharacterCaster::AbilityQHit()
{
	GetWorldTimerManager().SetTimer(AbilityQDurationTimerHandle, this, &ACharacterCaster::FinishAbilityQShield, AbilityQDuration);
	AbilityQMagicShield->SetCollisionProfileName("CharacterAttack");
	AbilityQMagicShield->SetVisibility(true);
}

void ACharacterCaster::AbilityEHit()
{
	if (AbilityEActorClass != nullptr)
	{
		for (int i = 0; i < 3; ++i)
		{
			AProjectile* Projectile = AProjectile::SpawnProjectile(AbilityEActorClass, GetActorTransform(),
				this, CombatStatus.Attack, GetDamageTypeClass());
			if (Projectile != nullptr)
			{
				FQuat NewRot = FQuat{ UKismetMathLibrary::RotatorFromAxisAndAngle(FVector::ZAxisVector, -20.0f + i * 20.0f) };
				Projectile->AddActorLocalRotation(NewRot);
			}
		}
	
	}
}

void ACharacterCaster::AbilityRHit()
{
	GetWorldTimerManager().SetTimer(AbilityRDurationTimerHandle, this, &ACharacterCaster::FinishAbilityR, AbilityRDuration);
	AbilityRMagicCylinder->SetCollisionProfileName("CharacterAttack");
	AbilityRMagicCylinder->SetVisibility(true);
}

void ACharacterCaster::AbilityRMBHit()
{
}

void ACharacterCaster::FinishAbilityQShield()
{
	AbilityQMagicShield->SetCollisionProfileName("NoCollision");
	AbilityQMagicShield->SetVisibility(false);
	AbilityQOverlapActors.Reset();
}

void ACharacterCaster::OnMagicShieldBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy == nullptr) return;

	if (AbilityQOverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().UnPauseTimer(AbilityQOverlapActors[OtherActor]);
	}
	else
	{
		FTimerHandle& TimerHandle = AbilityQOverlapActors.Add(OtherActor);
		AbilityQTickDamage(Enemy);
		FTimerDelegate AbilityQDamageTimerDelegate = FTimerDelegate::CreateUObject(this, &ACharacterCaster::AbilityQTickDamage, Enemy);
		GetWorldTimerManager().SetTimer(TimerHandle, AbilityQDamageTimerDelegate, 0.25f, true);
	}
}

void ACharacterCaster::OnMagicShieldEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AbilityQOverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().PauseTimer(AbilityQOverlapActors[OtherActor]);
	}
}

void ACharacterCaster::AbilityQTickDamage(AEnemy* Enemy)
{
	if (Enemy->GetEnemyState() == EEnemyState::EDeath)
	{
		GetWorldTimerManager().ClearTimer(AbilityQOverlapActors[Enemy]);
		AbilityQOverlapActors.Remove(Enemy);
	}
	else
	{
		ApplyDamage(Enemy, CombatStatus.Attack * 0.15f);
		if (AbilityQImpactEffect != nullptr)
		{
			UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AbilityQImpactEffect, FTransform{ Enemy->GetActorLocation() });
			Particle->SetRelativeScale3D(FVector(3.0f));
		}
	}
}

void ACharacterCaster::FinishAbilityR()
{
	AbilityRMagicCylinder->SetCollisionProfileName("NoCollision");
	AbilityRMagicCylinder->SetVisibility(false);
	StopAnimMontage(CharacterAnimationData.AbilityRMontage);
	AbilityROverlapActors.Reset();
	AttackEnd();
}

void ACharacterCaster::OnAbilityRBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy == nullptr) return;

	if (AbilityROverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().UnPauseTimer(AbilityROverlapActors[OtherActor]);
	}
	else
	{
		FTimerHandle& TimerHandle = AbilityROverlapActors.Add(OtherActor);
		AbilityRTickDamage(Enemy);
		FTimerDelegate AbilityRDamageTimerDelegate = FTimerDelegate::CreateUObject(this, &ACharacterCaster::AbilityRTickDamage, Enemy);
		GetWorldTimerManager().SetTimer(TimerHandle, AbilityRDamageTimerDelegate, 0.5f, true);
	}
}

void ACharacterCaster::OnAbilityREndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AbilityROverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().PauseTimer(AbilityROverlapActors[OtherActor]);
	}
}

void ACharacterCaster::AbilityRTickDamage(AEnemy* Enemy)
{
	if (Enemy->GetEnemyState() == EEnemyState::EDeath)
	{
		GetWorldTimerManager().ClearTimer(AbilityROverlapActors[Enemy]);
		AbilityROverlapActors.Remove(Enemy);
	}
	else
	{
		ApplyDamage(Enemy, CombatStatus.Attack * 1.0f);
	}
}
