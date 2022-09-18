// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "BaseCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRange = CreateDefaultSubobject<UCapsuleComponent>("Pickup Range");
	SetRootComponent(PickupRange);
	
	PickupRange->SetCapsuleRadius(50.0f);
	PickupRange->SetCapsuleHalfHeight(100.f);
	PickupRange->SetCollisionProfileName("Item");
	PickupRange->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemEffect != nullptr)
	{
		Effect = UNiagaraFunctionLibrary::SpawnSystemAttached(ItemEffect, PickupRange, "None",
			FVector(0.0f, 0.0f, -PickupRange->GetScaledCapsuleHalfHeight()), FRotator(), EAttachLocation::KeepRelativeOffset, true);
		Effect->SetRelativeScale3D(ItemEffectScale);
	}

	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapEvent);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* OverlapCharacter = Cast<ABaseCharacter>(OtherActor);
	if (OverlapCharacter != nullptr)
	{
		ApplyItemEffect(OverlapCharacter);
		if (PickupItemEffect != nullptr)
		{
			PickupEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(PickupItemEffect, OverlapCharacter->GetMesh(), "root",
				OverlapCharacter->GetActorLocation(), OverlapCharacter->GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
			PickupEffect->SetRelativeScale3D(PickupItemEffectScale);
		}
	}
	GetWorld()->DestroyActor(this);
	Effect->DestroyComponent();
}

