// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;
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
	virtual void Jump();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter = nullptr;
};
