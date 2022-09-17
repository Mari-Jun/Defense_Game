// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseCharacterAnimInstance.h"
#include "BasePlayerController.h"
#include "CrosshairWidget.h"
#include "CharacterStatusWidget.h"
#include "CharacterSkillTimeWidget.h"
#include "CriticalDamageType.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = 255;

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

				ChangeHPDelegate.AddDynamic(StatusWidget, &UCharacterStatusWidget::OnChangeHP);
				ChangeShieldDelegate.AddDynamic(StatusWidget, &UCharacterStatusWidget::OnChangeShield);
				ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);
				ChangeShieldDelegate.Broadcast(CombatStatus.CurrentShield, CombatStatus.MaxShield);
			}
		}
	}

	HitShakeCameraDelegate.AddDynamic(this, &ABaseCharacter::ShakePlayerCamera);
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

	if (CharacterState == ECharacterState::EDeath && CharacterStatusData.RespawnTimerHandle.IsValid())
	{
		float Time = GetWorldTimerManager().GetTimerElapsed(CharacterStatusData.RespawnTimerHandle);
		StatusWidget->SetRespawnTime(Time, CharacterStatusData.RespawnTime);
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
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void ABaseCharacter::Attack()
{
	if (CharacterAnimationData.AttackAnimMontange.IsEmpty() == false &&
		CharacterState == ECharacterState::EDefault)
	{
		int32 index = FMath::RandRange(0, CharacterAnimationData.AttackAnimMontange.Num() - 1);
		const auto& attack_montage = CharacterAnimationData.AttackAnimMontange[index];
		PlayAnimMontage(attack_montage);
		SetCharacterState(ECharacterState::EAttack);
		AttackState = EAttackState::EAttackLMB;
	}
}

void ABaseCharacter::AbilityQ(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityQMontage != nullptr &&
		CharacterState == ECharacterState::EDefault && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityQMontage);
		SetCharacterState(ECharacterState::EAttack);
		AttackState = EAttackState::EAbilityQ;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityE(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityEMontage != nullptr &&
		CharacterState == ECharacterState::EDefault && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityEMontage);
		SetCharacterState(ECharacterState::EAttack);
		AttackState = EAttackState::EAbilityE;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityR(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityRMontage != nullptr &&
		CharacterState == ECharacterState::EDefault && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMontage);
		SetCharacterState(ECharacterState::EAttack);
		AttackState = EAttackState::EAbilityR;

		StartAbilityCooldown(AbilityIndex);
	}
}

void ABaseCharacter::AbilityRMB(int32 AbilityIndex)
{
	if (CharacterAnimationData.AbilityRMBMontage != nullptr &&
		CharacterState == ECharacterState::EDefault && CheckAbilityCooldown(AbilityIndex))
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMBMontage);
		SetCharacterState(ECharacterState::EAttack);
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

void ABaseCharacter::PlayHitReaction(float HitYaw)
{
	HitShakeCameraDelegate.Broadcast();

	UAnimMontage* HitReaction = CharacterAnimationData.HitReactionFWDAnimMontage;

	if (HitYaw >= -45.f && HitYaw <= 45.f)
	{
		if (CharacterAnimationData.HitReactionFWDAnimMontage != nullptr)
		{
			HitReaction = CharacterAnimationData.HitReactionFWDAnimMontage;
		}
	}
	else if (HitYaw >= -135.f && HitYaw < -45.f)
	{
		if (CharacterAnimationData.HitReactionLeftAnimMontage != nullptr)
		{
			HitReaction = CharacterAnimationData.HitReactionLeftAnimMontage;
		}
	}
	else if (HitYaw <= 135.f && HitYaw > 45.f)
	{
		if (CharacterAnimationData.HitReactionRightAnimMontage != nullptr)
		{
			HitReaction = CharacterAnimationData.HitReactionRightAnimMontage;
		}
	}
	else
	{
		if (CharacterAnimationData.HitReactionBWDAnimMontage != nullptr)
		{
			HitReaction = CharacterAnimationData.HitReactionBWDAnimMontage;
		}
	}

	if (HitReaction != nullptr)
	{
		PlayAnimMontage(HitReaction);
		SetCharacterState(ECharacterState::EReaction);
	}
}

void ABaseCharacter::ShakePlayerCamera()
{
	if (HitCameraShakeClass != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(HitCameraShakeClass, 1.0f);
	}
}

void ABaseCharacter::ResetAbilityTimer(int32 AbilityIndex)
{
	GetWorldTimerManager().ClearTimer(CharacterStatusData.AbilityTimerHandle[AbilityIndex]);
	StatusWidget->GetAbilityWidget(AbilityIndex)->SetCooldownVisibility(false);
}

void ABaseCharacter::KillObject()
{
	GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CharacterAnimationData.DeathMontage != nullptr)
	{
		PlayAnimMontage(CharacterAnimationData.DeathMontage);
	}
	SetCharacterState(ECharacterState::EDeath);
}

void ABaseCharacter::RespawnCharacter()
{
	SetCharacterState(ECharacterState::EDefault);
	GetMesh()->GlobalAnimRateScale = 1.f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetWorld()->GetFirstPlayerController()->Possess(this);

	CombatStatus.CurrentHP = CombatStatus.MaxHP;
	ChangeHPDelegate.Broadcast(CombatStatus.CurrentHP, CombatStatus.MaxHP);
	StatusWidget->SetRespawnWidgetVisibility(false);
}

void ABaseCharacter::AttackEnd()
{
	SetCharacterState(ECharacterState::EDefault);
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
	GetWorldTimerManager().SetTimer(CharacterStatusData.RespawnTimerHandle,
		this, &ABaseCharacter::RespawnCharacter, CharacterStatusData.RespawnTime);
	StatusWidget->SetRespawnWidgetVisibility(true);
}

void ABaseCharacter::ReactionEnd()
{
	SetCharacterState(ECharacterState::EDefault);
}

void ABaseCharacter::SetCharacterState(ECharacterState State)
{
	switch (State)
	{
	case ECharacterState::EDefault:
		break;
	case ECharacterState::EAttack:
		break;
	case ECharacterState::EReaction:
		break;
	case ECharacterState::EDeath:
		break;
	default: break;
	}

	CharacterState = State;
}
