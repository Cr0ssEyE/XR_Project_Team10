// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterBase.h"

// Sets default values
AKWBossMonsterBase::AKWBossMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKWBossMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKWBossMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

