// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseObject.h"

#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADefenseObject::ADefenseObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADefenseObject::BeginPlay()
{
	Super::BeginPlay();
	
}

float ADefenseObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float HitYaw = 0.f;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		const FRotator ActorRotation = GetActorRotation();
		const FRotator ShotRotation = UKismetMathLibrary::MakeRotFromX(PointDamageEvent->ShotDirection);

		HitYaw = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, ShotRotation).Yaw;
	}

	float Critical = DamageEvent.StaticStruct()->GetFloatMetaData("Critical");

	DamageAmount = TakeDamage(DamageAmount, Critical, HitYaw);

	if (Controller != nullptr)
	{
		Controller->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return DamageAmount;
}

float ADefenseObject::TakeDamage(float Attack, float Critical, float HitYaw)
{
	float DamageAmount = Attack;
	float DecresePercent = 0.0f;

	if (CombatStatus.Defense <= 100.f)
	{
		float Alpha = CombatStatus.Defense / 100.f;
		DecresePercent = FMath::Lerp<float>(0.0f, 0.5f, Alpha);
	}
	else if (CombatStatus.Defense > 100.f && CombatStatus.Defense <= 200.f)
	{
		float Alpha = (CombatStatus.Defense - 100.f) / 100.f;
		DecresePercent = FMath::Lerp<float>(0.5f, 0.8f, Alpha);
	}
	else
	{
		float Alpha = (CombatStatus.Defense - 200.f) / 800.f;
		DecresePercent = FMath::Lerp<float>(0.8f, 0.95f, Alpha);
	}

	DamageAmount -= DamageAmount * DecresePercent;

	float RandCritical = FMath::FRandRange(0.0f, 1.0f);
	if (FMath::IsNearlyZero(Critical) == false && RandCritical <= Critical)
		DamageAmount *= 2.0f;

	if (DamageAmount > 0.0f && CombatStatus.CurrentShield > 0.0f)
	{
		float ShieldDamage = FMath::Min(DamageAmount, CombatStatus.CurrentShield);
		DamageAmount -= ShieldDamage;

		CombatStatus.CurrentShield -= ShieldDamage;
		ChangeShieldDelegate.Broadcast(CombatStatus.CurrentShield, CombatStatus.MaxShield);
	}

	if (DamageAmount > 0.0f)
	{
		CombatStatus.CurrentHP = UKismetMathLibrary::FClamp(CombatStatus.CurrentHP - DamageAmount, 0.0f, CombatStatus.MaxHP);
		ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);

		CombatStatus.CurrentReactionValue += DamageAmount;
		ChangeReactionDelegate.Broadcast(CombatStatus.CurrentReactionValue, CombatStatus.ReactionValue);

		if (CombatStatus.CurrentReactionValue >= CombatStatus.ReactionValue)
		{
			CombatStatus.CurrentReactionValue -= CombatStatus.ReactionValue;
			PlayHitReaction(HitYaw);
		}

		if (CombatStatus.CurrentHP <= 0.0f)
		{
			KillObject();
		}
	}

	return DamageAmount;
}

void ADefenseObject::PlayHitReaction(float HitYaw)
{
}

void ADefenseObject::ApplyDamage(AActor* OtherActor, float Damage)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetController(), this, UDamageType::StaticClass());

}

void ADefenseObject::DisableCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ADefenseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADefenseObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}