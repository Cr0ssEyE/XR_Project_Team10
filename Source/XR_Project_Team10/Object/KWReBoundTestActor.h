// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "XR_Project_Team10/Enumeration/KWObjectType.h"
#include "KWReBoundTestActor.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWReBoundTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AKWReBoundTestActor();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY()
	EReBoundObjectType ReBoundObject;

	UPROPERTY()
	FVector CollisionDirection;
};
