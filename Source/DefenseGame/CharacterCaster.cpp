// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCaster.h"
#include "Projectile.h"

#include "Animation/BlendSpace1D.h"

#include "Kismet/GameplayStatics.h"

ACharacterCaster::ACharacterCaster()
{
	USkeletalMesh* caster_skeletal_mesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr,
		TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Skins/Tier_1_5/Pandapack/Meshes/Phase_Pandapack_GDC.Phase_Pandapack_GDC'")));
	GetMesh()->SetSkeletalMesh(caster_skeletal_mesh);

	CharacterAnimationData.IdleAnimSequence = Cast<UAnimSequence>(StaticLoadObject(UAnimSequence::StaticClass(), nullptr,
		TEXT("AnimSequence'/Game/_Game/Characters/Caster/Animations/Caster_Idle.Caster_Idle'")));
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
}

void ACharacterCaster::AbilityR()
{
	if (CharacterAnimationData.AbilityRMontage != nullptr)
	{
		if (AttackState == EAttackState::ENone)
		{
			PlayAnimMontage(CharacterAnimationData.AbilityRMontage);
			AttackState = EAttackState::EAbilityR;
			bIsAttackFull = true;
		}
		else if (AttackState == EAttackState::EAbilityR)
		{
			StopAnimMontage(CharacterAnimationData.AbilityRMontage);
			AttackEnd();
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

					AProjectile* SpawnBall = GetWorld()->SpawnActor<AProjectile>(AttackMagicBall, SpawnPoint, FRotator{});

					const FVector Start{ CrosshairWorldPosition };
					const FVector End{ Start + CrosshairWorldDirection * 50'000 };

					FVector ImpulseWorldDirection = CrosshairWorldDirection;

					FHitResult HitResult;
					if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
					{
						ImpulseWorldDirection = HitResult.ImpactPoint - SpawnPoint;
						ImpulseWorldDirection.Normalize();
					}

					SpawnBall->AddImpulse(ImpulseWorldDirection);
				}
			}
		}
	}
}

void ACharacterCaster::AbilityQHit()
{
}

void ACharacterCaster::AbilityEHit()
{
}

void ACharacterCaster::AbilityRHit()
{
}

void ACharacterCaster::AbilityRMBHit()
{
}