// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FaceToTarget.generated.h"

class AAIController;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UBTTask_FaceToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FaceToTarget(const FObjectInitializer& ObjectInitializer);
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void FaceToTarget(const FVector& TargetLocation, AAIController* Controller);

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 20.0f;
};
