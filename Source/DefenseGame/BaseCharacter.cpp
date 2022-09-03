// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseCharacterAnimInstance.h"
#include "BasePlayerController.h"
#include "CrosshairWidget.h"
#include "CharacterStatusWidget.h"
#include "CharacterSkillTimeWidget.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation({0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()});
	GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

	GetCapsuleComponent()->SetCollisionProfileName("Character");

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("camera boom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("follow camera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	ChangeAbilityTimeDelegate.SetNum(4);
	CharacterStatusData.AbilityTimerHandle.SetNum(4);
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CrosshairWidget != nullptr)
	{
		CrosshairWidget->RemoveFromParent();
		CrosshairWidget = nullptr;
	}

	if (StatusWidget != nullptr)
	{
		StatusWidget->RemoveFromParent();
		StatusWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CrosshairWidgetClass != nullptr)
	{
		ABasePlayerController* BPC = GetController<ABasePlayerController>();
		if (BPC != nullptr)
		{
			CrosshairWidget = CreateWidget<UCrosshairWidget>(BPC, CrosshairWidgetClass);
			if (CrosshairWidget != nullptr)
			{
				CrosshairWidget->AddToPlayerScreen();
			}
		}
	}

	if (StatusWidgetClass != nullptr)
	{
		ABasePlayerController* BPC = GetController<ABasePlayerController>();
		if (BPC != nullptr)
		{
			StatusWidget = CreateWidget<UCharacterStatusWidget>(BPC, StatusWidgetClass);
			if (StatusWidget != nullptr)
			{
				StatusWidget->SetBaseCharacter(this);
				StatusWidget->AddToPlayerScreen();
			}
		}
	}

	ChangeHPDelegate.Broadcast(CharacterStatusData.CurrentHP, CharacterStatusData.MaxHP);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 Index = 0; Index < CharacterStatusData.AbilityTimerHandle.Num(); ++Index)
	{
		if (CharacterStatusData.AbilityTimerHandle[Index].IsValid())
		{
			float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(CharacterStatusData.AbilityTimerHandle[Index]);
			ChangeAbilityTimeDelegate[Index].Broadcast(CharacterStatusData.AbilityTime[Index] - ElapsedTime);
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ABaseCharacter::Attack);
	PlayerInputComponent->BindAction<TDelegate<void(int32)>>("AbilityQ", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityQ, 0);
	PlayerInputComponent->BindAction<TDelegate<void(int32)>>("AbilityE", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityE, 1);
	PlayerInputComponent->BindAction<TDelegate<void(int32)>>("AbilityR", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityR, 2);
	PlayerInputComponent->BindAction<TDelegate<void(int32)>>("AbilityRMB", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityRMB, 3);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CharacterStatusData.CurrentHP -= DamageAmount;
	ChangeHPDelegate.Broadcast(CharacterStatusData.CurrentHP, CharacterStatusData.MaxHP);

	if (CharacterStatusData.CurrentHP <= 0)
	{
		KillCharacter();
	}

	return DamageAmount;
}

void ABaseCharacter::Attack()
{
	if (CharacterAnimationData.AttackAnimMontange.IsEmpty() == false &&
		AttackState == EAttackState::ENone)
	{
		int32 index = FMath::RandRange(0, CharacterAnimationData.AttackAnimMontange.Num() - 1);
		const auto& attack_montage = CharacterAnimationData.AttackAnimMontange[index];
		PlayAnimMontage(attack_montage);
		AttackState = EAttackState::EAttackLMB;
	}
}

void ABaseCharacter::AbilityQ(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityQMontage != nullptr &&
		AttackState == EAttackState::ENone && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityQMontage);
		AttackState = EAttackState::EAbilityQ;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityE(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityEMontage != nullptr &&
		AttackState == EAttackState::ENone && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityEMontage);
		AttackState = EAttackState::EAbilityE;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityR(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityRMontage != nullptr &&
		AttackState == EAttackState::ENone && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMontage);
		AttackState = EAttackState::EAbilityR;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityRMB(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityRMBMontage != nullptr &&
		AttackState == EAttackState::ENone && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMBMontage);
		AttackState = EAttackState::EAbilityRMB;

		StartAbilityCooldown(AbilityIndex);
	}
}

bool ABaseCharacter::CheckAbilityCooldown(int32 AbilityIndex) const
{
	return CharacterStatusData.AbilityTimerHandle[AbilityIndex].IsValid() == false;
}

void ABaseCharacter::StartAbilityCooldown(int32 AbilityIndex)
{
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ABaseCharacter::ResetAbilityTimer, AbilityIndex);
	GetWorldTimerManager().SetTimer(CharacterStatusData.AbilityTimerHandle[AbilityIndex],
		TimerDelegate, CharacterStatusData.AbilityTime[AbilityIndex], false);
	StatusWidget->GetAbilityWidget(AbilityIndex)->SetCooldownVisibility(true);
}

void ABaseCharacter::ResetAbilityTimer(int32 AbilityIndex)
{
	GetWorldTimerManager().ClearTimer(CharacterStatusData.AbilityTimerHandle[AbilityIndex]);
	StatusWidget->GetAbilityWidget(AbilityIndex)->SetCooldownVisibility(false);
}

void ABaseCharacter::KillCharacter()
{
	GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CharacterAnimationData.DeathMontage != nullptr)
	{
		PlayAnimMontage(CharacterAnimationData.DeathMontage);
	}
}

void ABaseCharacter::AttackEnd()
{
	AttackState = EAttackState::ENone;
	bIsAttackFull = false;
}

void ABaseCharacter::AttackHit()
{
	switch (AttackState)
	{
	case EAttackState::EAttackLMB: AttackLMBHit(); break;
	case EAttackState::EAbilityQ: AbilityQHit(); break;
	case EAttackState::EAbilityE: AbilityEHit(); break;
	case EAttackState::EAbilityR: AbilityRHit(); break;
	case EAttackState::EAbilityRMB: AbilityRMBHit(); break;
	default: break;
	}
}

void ABaseCharacter::FinishDeath()
{
	GetMesh()->GlobalAnimRateScale = 0.f;
}
