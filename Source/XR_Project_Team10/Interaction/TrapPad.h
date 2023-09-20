#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapPad.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ATrapPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapPad();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere)
	FVector TrapVector;
};
