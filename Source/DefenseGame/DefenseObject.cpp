// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseObject.h"
#include "CriticalDamageType.h"

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
	if (CombatStatus.CurrentHP <= 0.0f) return 0.f;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Controller != nullptr)
	{
		Controller->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	float HitYaw = 0.f;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		const FRotator ActorRotation = GetActorRotation();
		const FRotator ShotRotation = UKismetMathLibrary::MakeRotFromX(PointDamageEvent->ShotDirection);

		HitYaw = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, ShotRotation).Yaw;
	}

	float Critical = 0.0f;
	UCriticalDamageType* CriticalDamageType = Cast<UCriticalDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
	if (CriticalDamageType != nullptr)
	{
		 Critical = CriticalDamageType->Critical;
	}

	TakeDamage(DamageAmount, Critical, HitYaw);

	return DamageAmount;
}

void ADefenseObject::TakeDamage(float Attack, float Critical, float HitYaw)
{
	bool IsCritical = false;
	float DamageAmount = CalcDamage(Attack, Critical, CombatStatus.Defense, IsCritical);

	float HPDamage = DamageAmount, ShieldDamage = 0.f;

	if (DamageAmount > 0.0f && CombatStatus.CurrentShield > 0.0f)
	{
		ShieldDamage = FMath::Min(DamageAmount, CombatStatus.CurrentShield);
		HPDamage -= ShieldDamage;
		
		ChangeCombatStatusCurrentShield(-ShieldDamage);
	}

	if (HPDamage > 0.0f)
	{
		ChangeCombatStatusCurrentHP(-HPDamage);

		CombatStatus.CurrentReactionValue += HPDamage;
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

	BroadcastDamageInfoDelegate.Broadcast(HPDamage, ShieldDamage, IsCritical);
}

void ADefenseObject::PlayHitReaction(float HitYaw)
{
}

TSubclassOf<UDamageType> ADefenseObject::GetDamageTypeClass() const
{
	TSubclassOf<UDamageType> DamageTypeClass;

	auto DamageType = UCriticalDamageType::StaticClass();
	UCriticalDamageType* CriticalDamageType = Cast<UCriticalDamageType>(DamageType->GetDefaultObject());
	if (CriticalDamageType != nullptr)
	{
		CriticalDamageType->Critical = CombatStatus.Critical;
		DamageTypeClass = DamageType;
	}
	else
	{
		DamageTypeClass = UDamageType::StaticClass();
	}

	return DamageTypeClass;
}

void ADefenseObject::ApplyDamage(AActor* OtherActor, float Damage)
{
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetController(), this, GetDamageTypeClass());
}

void ADefenseObject::ApplyPointDamage(AActor* OtherActor, float Damage, const FVector& HitFromDirection, const FHitResult& HitInfo)
{
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, HitFromDirection,
		HitInfo, GetController(), this, GetDamageTypeClass());
}

void ADefenseObject::EnableCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

void ADefenseObject::ChangeCombatStatusCurrentHP(float DeltaCurrentHP)
{
	CombatStatus.CurrentHP = UKismetMathLibrary::FClamp(CombatStatus.CurrentHP + DeltaCurrentHP, 0.0f, CombatStatus.MaxHP);
	ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);
}

void ADefenseObject::ChangeCombatStatusMaxHP(float DeltaMaxHP)
{
	CombatStatus.MaxHP = FMath::Max(0.0f, CombatStatus.MaxHP + DeltaMaxHP);
	ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);
}

void ADefenseObject::ChangeCombatStatusCurrentShield(float DeltaCurrentShield)
{
	CombatStatus.CurrentShield = UKismetMathLibrary::FClamp(CombatStatus.CurrentShield + DeltaCurrentShield, 0.0f, CombatStatus.MaxShield);
	ChangeShieldDelegate.Broadcast(CombatStatus.CurrentShield, CombatStatus.MaxShield);
}

void ADefenseObject::ChangeCombatStatusMaxShield(float DeltaMaxShield)
{
	CombatStatus.MaxShield = FMath::Max(0.0f, CombatStatus.MaxShield + DeltaMaxShield);
	ChangeShieldDelegate.Broadcast(CombatStatus.CurrentShield, CombatStatus.MaxShield);
}

void ADefenseObject::ChangeCombatStatusAttack(float DeltaAttack)
{
	CombatStatus.Attack += DeltaAttack;
	ChangeAttackDelegate.Broadcast(CombatStatus.Attack);
}

void ADefenseObject::ChangeCombatStatusDefense(float DeltaDefense)
{
	CombatStatus.Defense += DeltaDefense;
	ChangeDefenseDelegate.Broadcast(CombatStatus.Defense);
}

void ADefenseObject::ChangeCombatStatusCritical(float DeltaCritical)
{
	CombatStatus.Critical += DeltaCritical;
	ChangeCriticalDelegate.Broadcast(CombatStatus.Critical);
}
