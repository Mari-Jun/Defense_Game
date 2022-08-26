// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyStatusWidget.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation({ 0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });
	GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

	GetMesh()->SetCollisionProfileName("Enemy");
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	EnemyStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("status widget");
	EnemyStatusWidgetComponent->SetupAttachment(GetRootComponent());
	EnemyStatusWidgetComponent->SetDrawSize({ 150.f, 20.f });
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyStatusWidgetComponent != nullptr)
	{
		EnemyStatusWidget = Cast<UEnemyStatusWidget>(EnemyStatusWidgetComponent->GetWidget());
		if (EnemyStatusWidget == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast GetWidget() to UEnemyStatusWidget"));
			GetWorld()->DestroyActor(this);
		}
		else
		{
			ChangeHPDelegate.AddDynamic(EnemyStatusWidget, &UEnemyStatusWidget::OnChangeHP);
			ChangeHPDelegate.Broadcast(EnemyStatusData.CurrentHP, EnemyStatusData.MaxHP);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyStatusWidgetComponent != nullptr && EnemyStatusWidgetComponent->IsVisible())
	{
		FVector WidgetLocation = EnemyStatusWidgetComponent->GetComponentLocation();
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
		EnemyStatusWidgetComponent->SetWorldRotation({ 0.0f, NewRotation.Yaw, 0.0f });
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		const FRotator ActorRotation = GetActorRotation();
		const FRotator ShotRotation = UKismetMathLibrary::MakeRotFromX(PointDamageEvent->ShotDirection);

		const float DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, ShotRotation).Yaw;
	}
	EnemyStatusData.CurrentHP -= DamageAmount;
	ChangeHPDelegate.Broadcast(EnemyStatusData.CurrentHP, EnemyStatusData.MaxHP);
	return DamageAmount;
}