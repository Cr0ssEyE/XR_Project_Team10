#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UENUM()
enum EJumpPadType : uint8 {
	E_Contact,	//접촉하면 점프
	E_Timing,		//타이밍에 맞게 점프 누르면 더 큰 점프
	E_Delay,		//일정 시간 후 점프
	E_PileDriver	//찍기
};

UCLASS()
class XR_PROJECT_TEAM10_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	AJumpPad();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(EditAnywhere)
	float JumpForceValue = 5;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EJumpPadType> JumpPadType;
};
