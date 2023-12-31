// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWLocationDetector.h"

// Sets default values
AKWLocationDetector::AKWLocationDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	HitCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCheckBox"));
	HitCheckBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitCheckBox->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	HitCheckBox->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AKWLocationDetector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKWLocationDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

