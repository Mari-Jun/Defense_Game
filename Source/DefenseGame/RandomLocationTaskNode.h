// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomLocationTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API URandomLocationTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	URandomLocationTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
