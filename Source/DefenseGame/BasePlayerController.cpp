// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "BaseCharacter.h"
#include "GameInterfaceWidget.h"
#include "GameResultWidget.h"

#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ABasePlayerController::BeginPlay()
{
	BaseCharacter = Cast<ABaseCharacter>(GetPawn());
	if (BaseCharacter == nullptr)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Could not cast GetPawn() to ABaseCharacter"));
	}
	DefaultTargetArmLength = BaseCharacter->GetCameraBoom()->TargetArmLength;

	if (GameInterfaceWidgetClass != nullptr)
	{
		GameInterfaceWidget = CreateWidget<UGameInterfaceWidget>(this, GameInterfaceWidgetClass);
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &ABasePlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ABasePlayerController::LookUp);
	InputComponent->BindAxis("Zoom", this, &ABasePlayerController::ZoomCamera);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ABasePlayerController::Jump);
	InputComponent->BindAction("ResetZoom", EInputEvent::IE_Pressed, this, &ABasePlayerController::ResetCameraZoom);
	FInputActionBinding& InputGameInterface = 
		InputComponent->BindAction("GameInterface", EInputEvent::IE_Pressed, this, &ABasePlayerController::ShowGameInterface);
	InputGameInterface.bExecuteWhenPaused = true;
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

void ABasePlayerController::ZoomCamera(float AxisValue)
{
	float ChangeValue = GetWorld()->GetDeltaSeconds() * AxisValue * ZoomCameraSpeed;
	float NewTargetArmLength = BaseCharacter->GetCameraBoom()->TargetArmLength + ChangeValue;
	NewTargetArmLength = UKismetMathLibrary::FClamp(NewTargetArmLength, ZoomCameraMinMaxValue.X, ZoomCameraMinMaxValue.Y);
	BaseCharacter->GetCameraBoom()->TargetArmLength = NewTargetArmLength;
}

void ABasePlayerController::Jump()
{
	BaseCharacter->Jump();
}

void ABasePlayerController::ResetCameraZoom()
{
	BaseCharacter->GetCameraBoom()->TargetArmLength = DefaultTargetArmLength;
}

void ABasePlayerController::ShowGameInterface()
{
	if (GameInterfaceWidget != nullptr)
	{
		if (GameInterfaceWidget->IsInViewport())
		{
			GameInterfaceWidget->RemoveFromParent();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			SetInputMode(FInputModeGameOnly());
			SetShowMouseCursor(false);
		}
		else
		{
			GameInterfaceWidget->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			SetInputMode(FInputModeGameAndUI());
			SetShowMouseCursor(true);
		}
	}
}

void ABasePlayerController::ShowGameResult(bool IsWin)
{
	if (GameResultWidgetClass != nullptr)
	{
		GameResultWidget = CreateWidget<UGameResultWidget>(this, GameResultWidgetClass);
		if (GameResultWidget != nullptr)
		{
			IsWin ? GameResultWidget->ShowWinResult() : GameResultWidget->ShowDefeatResult();
			GameResultWidget->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}