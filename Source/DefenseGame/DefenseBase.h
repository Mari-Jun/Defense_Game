// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatHelperInterface.h"
#include "DefenseBase.generated.h"

class UBaseStatusWidget;

class UCapsuleComponent;
class UWidgetComponent;

UCLASS()
class DEFENSEGAME_API ADefenseBase : public AActor, public ICombatHelperInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenseBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void BillboardStatusWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FDefaultCombatStatus CombatStatus;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* BaseStatusWidgetComponent;

	UBaseStatusWidget* BaseStatusWidget;

protected:
	FChangeHPDelegate ChangeHPDelegate;
	FChangeShieldDelegate ChangeShieldDelegate;
};
