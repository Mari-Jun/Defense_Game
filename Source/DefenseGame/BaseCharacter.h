// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UBlendSpace1D;

USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* IdleAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogStartBlendSpace;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogBlendSpace;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogStopBlendSpace;
};

UCLASS()
class DEFENSEGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:


protected:


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FCharacterAnimationData CharacterAnimationData;

};
