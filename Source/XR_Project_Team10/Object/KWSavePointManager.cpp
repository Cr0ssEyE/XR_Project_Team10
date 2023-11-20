// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWSavePointManager.h"

#include "XR_Project_Team10/Game/KWGameInstance.h"

// Sets default values
AKWSavePointManager::AKWSavePointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKWSavePointManager::BeginPlay()
{
	Super::BeginPlay();
	for (auto Point : SavePoints)
	{
		Point->RegisterSavePointDelegate.AddUObject(this, &AKWSavePointManager::RegisterSavePoint);
	}
	
	UKWGameInstance* GameInstance = GetWorld()->GetGameInstanceChecked<UKWGameInstance>();
	if(GameInstance->CheckSavePointActivate())
	{
		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(SavePoints[GameInstance->GetSavePointNum()]->GetSpawnLocation());
	}
}

// Called every frame
void AKWSavePointManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKWSavePointManager::RegisterSavePoint(AActor* SavePoint)
{
	AKWPlayerSavePoint* PlayerSavePoint = CastChecked<AKWPlayerSavePoint>(SavePoint);
	for (int p = 0; p < SavePoints.Num(); p++)
	{
		if(PlayerSavePoint == SavePoints[p])
		{
			UKWGameInstance* GameInstance = GetWorld()->GetGameInstanceChecked<UKWGameInstance>();
			GameInstance->SetSavePoint(p);
		}
	}
}

