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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpStartSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpApexSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpLandSequence; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) /**Use Additive Sequence*/
	UAnimSequence* JumpRecoverySequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AttackAnimMontange;
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
	virtual void Attack();

protected:


public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	bool bIsAttacking = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FCharacterAnimationData CharacterAnimationData;
	
public:
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
