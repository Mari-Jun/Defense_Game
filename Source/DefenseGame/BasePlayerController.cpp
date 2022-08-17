// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "BaseCharacter.h"

void ABasePlayerController::BeginPlay()
{
	BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (BaseCharacter == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Could not cast GetPawn() to ABaseCharacter"));
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasePlayerController::LookUp);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABasePlayerController::Jump);
}

void ABasePlayerController::MoveForward(float AxisValue)
{
	const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
	BaseCharacter->AddMovementInput(FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::X), AxisValue);
}

void ABasePlayerController::MoveRight(float AxisValue)
{
	const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
	BaseCharacter->AddMovementInput(FRotationMatrix{ YawRotation }.GetUnitAxis(EAxis::Y), AxisValue);
}

void ABasePlayerController::Turn(float AxisValue)
{
	AddYawInput(AxisValue);
}

void ABasePlayerController::LookUp(float AxisValue)
{
	AddPitchInput(AxisValue);
}

void ABasePlayerController::Jump()
{
	BaseCharacter->Jump();
}