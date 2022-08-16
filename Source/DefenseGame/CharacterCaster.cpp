// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCaster.h"

ACharacterCaster::ACharacterCaster()
{
	USkeletalMesh* caster_skeletal_mesh = Cast<USkeletalMesh>(StaticLoadObject(USkeletalMesh::StaticClass(), nullptr,
		TEXT("SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Skins/Tier_1_5/Pandapack/Meshes/Phase_Pandapack_GDC.Phase_Pandapack_GDC'")));
	GetMesh()->SetSkeletalMesh(caster_skeletal_mesh);
}
