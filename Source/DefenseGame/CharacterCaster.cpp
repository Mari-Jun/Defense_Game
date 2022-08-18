// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCaster.h"

#include "Animation/BlendSpace1D.h"

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

}
