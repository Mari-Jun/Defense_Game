// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseBase.h"
#include "BaseStatusWidget.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADefenseBase::ADefenseBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	CapsuleComponent->SetCollisionProfileName("Character");

	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("base mesh");
	BaseMesh->SetupAttachment(GetRootComponent());

	BaseStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("status widget");
	BaseStatusWidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADefenseBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (BaseStatusWidgetComponent != nullptr)
	{
		BaseStatusWidget = Cast<UBaseStatusWidget>(BaseStatusWidgetComponent->GetWidget());
		if (BaseStatusWidget == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast GetWidget() to UBaseStatusWidget"));
			GetWorld()->DestroyActor(this);
		}
		else
		{
			ChangeHPDelegate.AddDynamic(BaseStatusWidget, &UBaseStatusWidget::OnChangeHP);
			ChangeShieldDelegate.AddDynamic(BaseStatusWidget, &UBaseStatusWidget::OnChangeShield);
			ChangeHPDelegate.Broadcast(CurrentHP, MaxHP);
			ChangeShieldDelegate.Broadcast(CurrentShield, MaxShield);
		}
	}
}

// Called every frame
void ADefenseBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BillboardStatusWidget();
}

float ADefenseBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentShield <= 0.0f)
	{
		CurrentHP = UKismetMathLibrary::FClamp(CurrentHP - DamageAmount, 0.0f, MaxHP);
		ChangeHPDelegate.Broadcast(CurrentHP, MaxHP);
	}
	else
	{
		CurrentShield = UKismetMathLibrary::FClamp(CurrentShield - DamageAmount, 0.0f, MaxShield);
		ChangeShieldDelegate.Broadcast(CurrentShield, MaxShield);
	}

	return DamageAmount;
}

void ADefenseBase::BillboardStatusWidget()
{
	if (BaseStatusWidgetComponent != nullptr)
	{
		FVector WidgetLocation = BaseStatusWidgetComponent->GetComponentLocation();
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

		FVector NewLocation = UKismetMathLibrary::RotateAngleAxis(FVector{ 100.0f, 0.0f, 0.0f }, NewRotation.Yaw, FVector::ZAxisVector);
		FVector WidgetReleativeLocation = BaseStatusWidgetComponent->GetRelativeLocation();
		WidgetReleativeLocation.X = WidgetReleativeLocation.Y = 0.0f;

		BaseStatusWidgetComponent->SetRelativeRotation({ 0.0f, NewRotation.Yaw, 0.0f });
		BaseStatusWidgetComponent->SetRelativeLocation(WidgetReleativeLocation + NewLocation);
	}
}

