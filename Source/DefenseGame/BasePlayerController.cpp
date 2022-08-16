// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "BaseCharacter.h"

void ABasePlayerController::BeginPlay()
{
	if (Cast<ABaseCharacter>(GetPawn()) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not cast GetPawn() to ABaseCharacter"))
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasePlayerController::LookUp);
}

void ABasePlayerController::MoveForward(float AxisValue)
{
	const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
	GetPawn()->AddMovementInput(FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X), AxisValue);
}

void ABasePlayerController::MoveRight(float AxisValue)
{
	const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
	GetPawn()->AddMovementInput(FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y), AxisValue);
}

void ABasePlayerController::Turn(float AxisValue)
{
	AddYawInput(AxisValue);
}

void ABasePlayerController::LookUp(float AxisValue)
{
	AddPitchInput(AxisValue);
}
