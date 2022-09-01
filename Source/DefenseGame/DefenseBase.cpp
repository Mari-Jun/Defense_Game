// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseBase.h"

// Sets default values
ADefenseBase::ADefenseBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("base mesh");

	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ADefenseBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefenseBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

