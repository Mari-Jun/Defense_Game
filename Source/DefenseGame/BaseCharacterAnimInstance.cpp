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

		TurnInPlace();

		CharacterPitch = BaseCharacter->GetBaseAimRotation().Pitch;
	}
}

void UBaseCharacterAnimInstance::StartTurn()
{
	GetWorld()->GetTimerManager().SetTimer(TurnInPlaceTimer, 1.f, false);
}

void UBaseCharacterAnimInstance::StopTurn()
{
}

void UBaseCharacterAnimInstance::TurnInPlace()
{
	if (Speed > 0.0f || bIsInAir)
	{
		RootYawOffset = 0.0f;
		LastFrameTurnYaw = TurnYaw = BaseCharacter->GetActorRotation().Yaw;
		LastFrameRotationYawCurveValue = RotationYawCurveValue = -90.f;
	}
	else
	{
		LastFrameTurnYaw = TurnYaw;
		TurnYaw = BaseCharacter->GetActorRotation().Yaw;

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - (TurnYaw - LastFrameTurnYaw));

		const float Turning{ GetCurveValue("Turning") };
		if (Turning > 0 && BaseCharacter->GetTurnRotationCurve() != nullptr)
		{
			const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(TurnInPlaceTimer);

			LastFrameRotationYawCurveValue = RotationYawCurveValue;
			RotationYawCurveValue = BaseCharacter->GetTurnRotationCurve()->GetFloatValue(ElapsedTime);

			const float DeltaRotationYaw = RotationYawCurveValue - LastFrameRotationYawCurveValue;
			(RootYawOffset > 0) ? RootYawOffset -= DeltaRotationYaw : RootYawOffset += DeltaRotationYaw;

			const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
			if (ABSRootYawOffset > 90.f)
			{
				const float YawExcess = ABSRootYawOffset - 90.f;
				(RootYawOffset > 0) ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
		else
		{
			LastFrameRotationYawCurveValue = RotationYawCurveValue = -90.f;
		}
	}
}