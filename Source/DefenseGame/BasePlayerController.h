// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;
class UGameInterfaceWidget;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void SetupInputComponent() override;

private:
	virtual void MoveForward(float AxisValue);
	virtual void MoveRight(float AxisValue);
	virtual void Turn(float AxisValue);
	virtual void LookUp(float AxisValue);
	virtual void ZoomCamera(float AxisValue);

	virtual void Jump();
	void ResetCameraZoom();
	void ShowGameInterface();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float DefaultTargetArmLength = 450.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float ZoomCameraSpeed = 2000.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	FVector2D ZoomCameraMinMaxValue = { 200.f, 800.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameInterfaceWidget> GameInterfaceWidgetClass;

	UGameInterfaceWidget* GameInterfaceWidget;
};
