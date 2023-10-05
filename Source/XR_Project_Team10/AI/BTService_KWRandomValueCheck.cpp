// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/AI/BTService_KWRandomValueCheck.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_KWRandomValueCheck::UBTService_KWRandomValueCheck()
{
	
}

void UBTService_KWRandomValueCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsInt(BlackboardKey.SelectedKeyName, FMath::RandRange(0, RandMax));
}