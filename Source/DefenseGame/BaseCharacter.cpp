// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseCharacterAnimInstance.h"

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

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ABaseCharacter::Attack()
{
	if (CharacterAnimationData.AttackAnimMontange.IsEmpty() == false && bIsAttacking == false)
	{
		int32 index = FMath::RandRange(0, CharacterAnimationData.AttackAnimMontange.Num() - 1);
		const auto& attack_montage = CharacterAnimationData.AttackAnimMontange[index];
		PlayAnimMontage(attack_montage);
		bIsAttacking = true;
	}
}

void ABaseCharacter::AttackEnd()
{
	bIsAttacking = false;
}

void ABaseCharacter::AttackHit()
{
}
