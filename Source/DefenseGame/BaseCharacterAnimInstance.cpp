// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

UBaseCharacterAnimInstance::UBaseCharacterAnimInstance()
{
}

void UBaseCharacterAnimInstance::NativeBeginPlay()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn != nullptr)
	{
		BaseCharacter = Cast<ABaseCharacter>(OwnerPawn);
		if (BaseCharacter == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast TryGetPawnOwner() to ABaseCharacter"));
		}
	}
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (BaseCharacter != nullptr)
	{
		Speed = BaseCharacter->GetVelocity().Length();
		bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
		bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0;

		if (Speed > 0.0f)
		{
			MovementOffset = UKismetMathLibrary::NormalizedDeltaRotator(BaseCharacter->GetBaseAimRotation(),
				UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity()));
		}
	}
}
