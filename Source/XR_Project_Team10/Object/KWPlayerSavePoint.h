// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "KWPlayerSavePoint.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FRegisterSavePointDelegate, AActor*)

UCLASS()
class XR_PROJECT_TEAM10_API AKWPlayerSavePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWPlayerSavePoint();

	FRegisterSavePointDelegate RegisterSavePointDelegate;

public:
	FORCEINLINE FVector GetSpawnLocation() const { return PlayerSpawnLocation->GetComponentLocation(); }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> PlayerSpawnLocation;

private:
	uint8 bIsTriggered : 1;
};
