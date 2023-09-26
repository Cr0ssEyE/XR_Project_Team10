#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoostPad.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ABoostPad : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoostPad();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere)
	float AccelarationValue = 5;
};
