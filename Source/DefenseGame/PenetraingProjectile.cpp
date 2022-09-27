// Fill out your copyright notice in the Description page of Project Settings.


#include "PenetraingProjectile.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"

void APenetraingProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void APenetraingProjectile::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().UnPauseTimer(OverlapActors[OtherActor]);
	}
	else
	{
		FTimerHandle& TimerHandle = OverlapActors.Add(OtherActor);
		TickDamage(OtherActor);
		FTimerDelegate DamageTimerDelegate = FTimerDelegate::CreateUObject(this, &APenetraingProjectile::TickDamage, OtherActor);
		GetWorldTimerManager().SetTimer(TimerHandle, DamageTimerDelegate, DamageTickTime, true);
	}

}

void APenetraingProjectile::OnEndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlapActors.Contains(OtherActor))
	{
		GetWorldTimerManager().PauseTimer(OverlapActors[OtherActor]);
	}
}

void APenetraingProjectile::TickDamage(AActor* OtherActor)
{
	if (OwnerCharacter != nullptr)
	{
		UGameplayStatics::ApplyDamage(OtherActor, AttackDamage, OwnerCharacter->GetController(), this, DamageType);	
	}
}
