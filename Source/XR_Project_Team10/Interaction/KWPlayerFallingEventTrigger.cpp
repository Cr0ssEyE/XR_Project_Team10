// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Interaction/KWPlayerFallingEventTrigger.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

// Sets default values
AKWPlayerFallingEventTrigger::AKWPlayerFallingEventTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	TriggerBox->SetCollisionProfileName(CP_GIMMICK);
	RootComponent = TriggerBox;
}

// Called when the game starts or when spawned
void AKWPlayerFallingEventTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKWPlayerFallingEventTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(OtherActor);
	if(PlayerCharacter)
	{
		PlayerCharacter->SetActorLocation(ReSpawnPosition->GetActorLocation());
		PlayerCharacter->GetMeshComp()->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	}
}


