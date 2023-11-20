// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "KWPlayerFallingEventTrigger.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWPlayerFallingEventTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWPlayerFallingEventTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
private:
	UPROPERTY(EditAnywhere, DisplayName = "낙하 체크 박스")
	TObjectPtr<UBoxComponent> TriggerBox;
	
	UPROPERTY(EditAnywhere, DisplayName = "재생성 위치")
	TObjectPtr<USceneComponent> ReSpawnPosition;
	
};
