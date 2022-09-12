// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefenseBase.generated.h"

class UBaseStatusWidget;

class UCapsuleComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeDefenseBaseHPDelegate, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeDefenseBaseShieldDelegate, float, CurrentShield, float, MaxShield);

UCLASS()
class DEFENSEGAME_API ADefenseBase : public AActor
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

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* BaseStatusWidgetComponent;

	UBaseStatusWidget* BaseStatusWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float CurrentHP = 5000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxHP = 5000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float CurrentShield = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxShield = 500.f;

public:
	FChangeDefenseBaseHPDelegate ChangeHPDelegate;
	FChangeDefenseBaseShieldDelegate ChangeShieldDelegate;

};
