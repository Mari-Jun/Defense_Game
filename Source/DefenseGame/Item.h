// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class ABaseCharacter;

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class DEFENSEGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ApplyItemEffect(ABaseCharacter* Character);

	virtual void ApplyItemEffect_Implementation(ABaseCharacter* Character) {}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ItemEffect;
	UNiagaraComponent* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* PickupItemEffect;
	UNiagaraComponent* PickupEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	FVector ItemEffectScale = FVector(1.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	FVector PickupItemEffectScale = FVector(1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	USphereComponent* PickupRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	int32 ItemLevel = 1;
};
