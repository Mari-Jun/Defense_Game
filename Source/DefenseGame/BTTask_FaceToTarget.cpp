// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FaceToTarget.h"
#include "AIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"

UBTTask_FaceToTarget::UBTTask_FaceToTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Face To";

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FaceToTarget, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FaceToTarget, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FaceToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (Blackboard == nullptr || Controller == nullptr || Controller->GetPawn() == nullptr) return EBTNodeResult::Failed;

	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
		if (TargetActor == nullptr) return EBTNodeResult::Failed;

		FVector TargetLocation = TargetActor->GetActorLocation();
		FaceToTarget(TargetLocation, Controller);
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FVector TargetLocation = Blackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		FaceToTarget(TargetLocation, Controller);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_FaceToTarget::FaceToTarget(const FVector& TargetLocation, AAIController* Controller)
{
	FVector OwnerPawnLocation = Controller->GetPawn()->GetActorLocation();
	FRotator OwnerPawnRotation = Controller->GetPawn()->GetActorRotation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerPawnLocation, TargetLocation);

	auto NewRotation = FMath::RInterpTo(OwnerPawnRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), RotateSpeed);
	NewRotation.Pitch = NewRotation.Roll = 0.0f;
	Controller->GetPawn()->SetActorRotation(NewRotation);
}

FString UBTTask_FaceToTarget::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
		BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}