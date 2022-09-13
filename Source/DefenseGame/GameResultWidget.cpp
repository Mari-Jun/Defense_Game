// Fill out your copyright notice in the Description page of Project Settings.


#include "GameResultWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GameplayStatics.h"

void UGameResultWidget::ShowWinResult()
{
	WinText->SetVisibility(ESlateVisibility::Visible);
	WinImage->SetVisibility(ESlateVisibility::Visible);
	DefeatText->SetVisibility(ESlateVisibility::Hidden);
	DefeatImage->SetVisibility(ESlateVisibility::Hidden);
}

void UGameResultWidget::ShowDefeatResult()
{
	WinText->SetVisibility(ESlateVisibility::Hidden);
	WinImage->SetVisibility(ESlateVisibility::Hidden);
	DefeatText->SetVisibility(ESlateVisibility::Visible);
	DefeatImage->SetVisibility(ESlateVisibility::Visible);
}

void UGameResultWidget::NativeOnInitialized()
{
	ExitButton->OnClicked.AddDynamic(this, &UGameResultWidget::QuitGame);
}

void UGameResultWidget::QuitGame()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UGameplayStatics::OpenLevel(GetWorld(), "LobbyMap");
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}
