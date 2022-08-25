// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UEnemyStatusWidget;

class UWidgetComponent;

USTRUCT(BlueprintType)
struct FEnemyStatusData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 200.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeEnemyHPDelegate, float, CurrentHP, float, MaxHP);

UCLASS()
class DEFENSEGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	virtual void OnCharacterAttackOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FEnemyStatusData EnemyStatusData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* EnemyStatusWidgetComponent;

	UEnemyStatusWidget* EnemyStatusWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* IdleAnimSequence;

public:
	FChangeEnemyHPDelegate ChangeHPDelegate;

};
