// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UCrosshairWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(LeftTexture->Slot);
	SetCrosshairThickness(PanelSlot, false);
	SetCrosshairOffset(PanelSlot, false, true);
	PanelSlot = Cast<UCanvasPanelSlot>(RightTexture->Slot);
	SetCrosshairThickness(PanelSlot, false);
	SetCrosshairOffset(PanelSlot, false, false);
	PanelSlot = Cast<UCanvasPanelSlot>(TopTexture->Slot);
	SetCrosshairThickness(PanelSlot, true);
	SetCrosshairOffset(PanelSlot, true, true);
	PanelSlot = Cast<UCanvasPanelSlot>(BottomTexture->Slot);
	SetCrosshairThickness(PanelSlot, true);
	SetCrosshairOffset(PanelSlot, true, false);

	LeftTexture->SetBrushTintColor(CrosshairColor);
	RightTexture->SetBrushTintColor(CrosshairColor);
	TopTexture->SetBrushTintColor(CrosshairColor);
	BottomTexture->SetBrushTintColor(CrosshairColor);
}

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UCrosshairWidget::SetCrosshairThickness(UCanvasPanelSlot* PanelSlot, bool IsVerticle)
{
	if (PanelSlot != nullptr)
	{
		FVector2D SlotSize = PanelSlot->GetSize();
		if (IsVerticle)
		{
			SlotSize.X = CrosshairThickness;
		}
		else
		{
			SlotSize.Y = CrosshairThickness;
		}
		PanelSlot->SetSize(SlotSize);
	}
}

void UCrosshairWidget::SetCrosshairOffset(UCanvasPanelSlot* PanelSlot, bool IsVerticle, bool IsInverse)
{
	if (PanelSlot != nullptr)
	{
		FVector2D SlotPosition = PanelSlot->GetPosition();
		if (IsVerticle)
		{
			SlotPosition.Y = IsInverse ? -CrosshairOfffset : CrosshairOfffset;
		}
		else
		{
			SlotPosition.X = IsInverse ? -CrosshairOfffset : CrosshairOfffset;
		}
		PanelSlot->SetPosition(SlotPosition);
	}
}
