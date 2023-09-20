// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Interaction/BoostPad.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ABoostPad::ABoostPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoostPad::BeginPlay()
{
	Super::BeginPlay();
	
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void ABoostPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto playableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != playableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("boost"));
		UPrimitiveComponent* physicalComp = Cast<UPrimitiveComponent>(playableCharacter->GetDefaultSubobjectByName(TEXT("CollisionAndPhysics")));
		physicalComp->SetPhysicsLinearVelocity(GetActorRotation().Vector() * 100.0f);
	}
}

void ABoostPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}

// Called every frame
void ABoostPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

