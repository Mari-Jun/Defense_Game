// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseCharacterAnimInstance.h"
#include "BasePlayerController.h"
#include "CrosshairWidget.h"
#include "CharacterStatusWidget.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation({0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()});
	GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

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

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ABaseCharacter::Attack);
	PlayerInputComponent->BindAction("AbilityQ", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityQ);
	PlayerInputComponent->BindAction("AbilityE", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityE);
	PlayerInputComponent->BindAction("AbilityR", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityR);
	PlayerInputComponent->BindAction("AbilityRMB", EInputEvent::IE_Pressed, this, &ABaseCharacter::AbilityRMB);
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

void ABaseCharacter::AbilityQ()
{
	if (CharacterAnimationData.AbilityQMontage != nullptr &&
		AttackState == EAttackState::ENone)
	{
		PlayAnimMontage(CharacterAnimationData.AbilityQMontage);
		AttackState = EAttackState::EAbilityQ;
	}
}

void ABaseCharacter::AbilityE()
{
	if (CharacterAnimationData.AbilityEMontage != nullptr &&
		AttackState == EAttackState::ENone)
	{
		PlayAnimMontage(CharacterAnimationData.AbilityEMontage);
		AttackState = EAttackState::EAbilityE;
	}
}

void ABaseCharacter::AbilityR()
{
	if (CharacterAnimationData.AbilityRMontage != nullptr &&
		AttackState == EAttackState::ENone)
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMontage);
		AttackState = EAttackState::EAbilityR;
	}
}

void ABaseCharacter::AbilityRMB()
{
	if (CharacterAnimationData.AbilityRMBMontage != nullptr &&
		AttackState == EAttackState::ENone)
	{
		PlayAnimMontage(CharacterAnimationData.AbilityRMBMontage);
		AttackState = EAttackState::EAbilityRMB;
	}
}

void ABaseCharacter::AttackEnd()
{
	AttackState = EAttackState::ENone;
	bIsAttackFull = false;

	//실험을 위한 코드
	ChangeHPDelegate.Broadcast(--CharacterStatusData.CurrentHP, CharacterStatusData.MaxHP);
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
