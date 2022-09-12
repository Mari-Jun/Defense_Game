// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInterfaceWidget.h"

#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

void UGameInterfaceWidget::NativeOnInitialized()
{
	ResumeButton->OnClicked.AddDynamic(this, &UGameInterfaceWidget::ResumeGame);
	ExitButton->OnClicked.AddDynamic(this, &UGameInterfaceWidget::QuitGame);
}

void UGameInterfaceWidget::NativePreConstruct()
{
}

void UGameInterfaceWidget::ResumeGame()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UGameInterfaceWidget::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UGameplayStatics::OpenLevel(GetWorld(), "LobbyMap");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}
