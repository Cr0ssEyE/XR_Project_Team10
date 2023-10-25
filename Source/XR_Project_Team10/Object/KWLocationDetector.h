// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "KWLocationDetector.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AKWLocationDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWLocationDetector();
	FORCEINLINE void SetTargetCharacter(ACharacter* Target) { TargetPlayer = Target; }
	FORCEINLINE ACharacter* GetTargetCharacter() { return TargetPlayer; }
	FORCEINLINE void SetHitBoxExtent(const float BoxExtent) const { HitCheckBox->SetBoxExtent(FVector(BoxExtent, BoxExtent, BoxExtent)); }
	FORCEINLINE void SetHitBoxExtent(const FVector& BoxExtent) const { HitCheckBox->SetBoxExtent(BoxExtent); }
	FORCEINLINE void SetHitChannelType(const ECollisionChannel CollisionChannel) const { HitCheckBox->SetCollisionObjectType(CollisionChannel); }
	FORCEINLINE void SetHitChannelProfile(const FName ProfileName) const { HitCheckBox->SetCollisionProfileName(ProfileName); }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCheckBox;
	
	UPROPERTY()
	TObjectPtr<ACharacter> TargetPlayer;
	
};
