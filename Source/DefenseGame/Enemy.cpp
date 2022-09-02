// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyStatusWidget.h"
#include "EnemyController.h"
#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	EnemyStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("status widget");
	EnemyStatusWidgetComponent->SetupAttachment(GetRootComponent());
	EnemyStatusWidgetComponent->SetDrawSize({ 150.f, 20.f });
	HideStatusWidget();

	AttackRangeSphereComponent = CreateDefaultSubobject<USphereComponent>("AttackRangeSphere");
	AttackRangeSphereComponent->SetupAttachment(GetRootComponent());
	AttackRangeSphereComponent->SetSphereRadius(150.f);
	AttackRangeSphereComponent->SetCollisionProfileName("EnemyAttack");
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

	EnemyController = Cast<AEnemyController>(GetController());
	if (EnemyController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not cast GetController() to AEnemyController"));
		GetWorld()->DestroyActor(this);
	}

	if (AttackRangeSphereComponent != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not cast GetController() to AEnemyController"));
		AttackRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackRangeBeginOverlap);
		AttackRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackRangeEndOverlap);
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

	if (CheckAttack())
	{
		Attack();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DeltaYaw = 0.f;
	
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		const FRotator ActorRotation = GetActorRotation();
		const FRotator ShotRotation = UKismetMathLibrary::MakeRotFromX(PointDamageEvent->ShotDirection);

		DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, ShotRotation).Yaw;
	}
	EnemyStatusData.CurrentHP -= DamageAmount;
	EnemyStatusData.CurrentReactionValue += DamageAmount;
	ChangeHPDelegate.Broadcast(EnemyStatusData.CurrentHP, EnemyStatusData.MaxHP);

	PlayHitReaction(DeltaYaw);

	ShowStatusWidget();

	if (EnemyStatusData.CurrentHP <= 0.f)
	{
		KillEnemy();
	}

	return DamageAmount;
}

void AEnemy::FinishDeath()
{
	GetMesh()->GlobalAnimRateScale = 0.0f;
	HideStatusWidget();
	GetWorldTimerManager().SetTimer(DestoryActorTimerHandle, this, &AEnemy::DestoryEnemy, DeathTime, false);
}

void AEnemy::KillEnemy()
{
	EnemyController->KilledControlledPawn();
	DisableCollision();
	EnemyState = EEnemyState::EDeath;
	if (DeadAnimMontage != nullptr)
	{
		PlayAnimMontage(DeadAnimMontage);
	}
	else
	{
		GetWorld()->DestroyActor(this);
	}
}

void AEnemy::DestoryEnemy()
{
	GetWorld()->DestroyActor(this);
}

void AEnemy::ShowStatusWidget()
{
	EnemyStatusWidgetComponent->SetHiddenInGame(false);
	GetWorldTimerManager().SetTimer(ShowStatusWidgetTimerHandle, this, &AEnemy::HideStatusWidget, ShowStatusWidgetTime, false);
}

void AEnemy::HideStatusWidget()
{
	EnemyStatusWidgetComponent->SetHiddenInGame(true);
}

void AEnemy::PlayHitReaction(float HitYaw)
{
	if (EnemyStatusData.CurrentReactionValue < EnemyStatusData.ReactionValue) return;

	EnemyStatusData.CurrentReactionValue -= EnemyStatusData.ReactionValue;

	UAnimMontage* HitReaction = HitReactionFWDAnimMontage;

	if (HitYaw >= -45.f && HitYaw <= 45.f)
	{
		if (HitReactionFWDAnimMontage != nullptr)
		{
			HitReaction = HitReactionFWDAnimMontage;
		}
	}
	else if (HitYaw >= -135.f && HitYaw < -45.f)
	{
		if (HitReactionLeftAnimMontage != nullptr)
		{
			HitReaction = HitReactionLeftAnimMontage;
		}
	}
	else if (HitYaw <= 135.f && HitYaw > 45.f)
	{
		if (HitReactionRightAnimMontage != nullptr)
		{
			HitReaction = HitReactionRightAnimMontage;
		}
	}
	else
	{
		if (HitReactionBWDAnimMontage != nullptr)
		{
			HitReaction = HitReactionBWDAnimMontage;
		}
	}

	if (HitReaction != nullptr)
	{
		PlayAnimMontage(HitReaction);
		EnemyState = EEnemyState::EReaction;
	}
}

bool AEnemy::CheckAttack()
{
	if (EnemyStatusData.CanAttack == false) return false;
	if (EnemyState != EEnemyState::ENone) return false;

	return true;
}

void AEnemy::Attack()
{
	if (AttackAnimMontange.IsEmpty() == false)
	{
		int32 index = FMath::RandRange(0, AttackAnimMontange.Num() - 1);
		const auto& attack_montage = AttackAnimMontange[index];
		PlayAnimMontage(attack_montage);
		EnemyState = EEnemyState::EAttack;
	}
}

void AEnemy::OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (EnemyController != nullptr && OtherActor == EnemyController->GetTargetCharacter())
	{
		EnemyStatusData.CanAttack = true;
	}
}

void AEnemy::OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (EnemyController != nullptr &&
		(EnemyController->GetTargetCharacter() == nullptr || EnemyController->GetTargetCharacter() == OtherActor))
	{
		EnemyStatusData.CanAttack = false;
	}
}

void AEnemy::DisableCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackRangeSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::ENone;
}

void AEnemy::ReactionEnd()
{
	EnemyState = EEnemyState::ENone;
}
