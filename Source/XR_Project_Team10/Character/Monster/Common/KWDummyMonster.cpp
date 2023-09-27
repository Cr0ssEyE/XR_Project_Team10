// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Common/KWDummyMonster.h"
#include "XR_Project_Team10/AI/Common/KWCommonAIController.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
// Sets default values
AKWDummyMonster::AKWDummyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AKWCommonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetMesh()->SetSkeletalMesh(FPPConstructorHelper::FindAndGetObject<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/9-CommonAI/SkeletalMesh/NormalMonster2.NormalMonster2'")));
}

// Called when the game starts or when spawned
void AKWDummyMonster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKWDummyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKWDummyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

