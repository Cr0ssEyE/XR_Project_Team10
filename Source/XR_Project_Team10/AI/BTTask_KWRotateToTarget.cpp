// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/BTTask_KWRotateToTarget.h"
#include "XR_Project_Team10/Constant/KWBlackBoardKeyName.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_KWRotateToTarget::UBTTask_KWRotateToTarget()
{
	NodeName = TEXT("Rotate To Target");
}

EBTNodeResult::Type UBTTask_KWRotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KEY_TARGET));
	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	float TurnSpeed = 5.f;
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));
	
	return EBTNodeResult::Succeeded;
}
