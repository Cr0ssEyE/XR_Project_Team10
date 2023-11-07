// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XR_Project_Team10/Object/KWPlayerSavePoint.h"
#include "KWSavePointManager.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWSavePointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWSavePointManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void RegisterSavePoint(AKWPlayerSavePoint* SavePoint);
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<AKWPlayerSavePoint*> SavePoints;
	
};
