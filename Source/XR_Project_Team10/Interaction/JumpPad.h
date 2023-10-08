#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UENUM()
enum class EJumpPadType : uint8
{
	E_Contact UMETA(DisplayName = "접촉"),//접촉하면 점프
	E_Timing UMETA(DisplayName = "타이밍"),	//타이밍에 맞게 점프 누르면 더 큰 점프
	E_Delay UMETA(DisplayName = "지연"),	//일정 시간 후 점프
	E_PileDriver UMETA(DisplayName = "파일 드라이버")	//찍기
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
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditAnywhere, DisplayName = "점프 크기값")
	float JumpForceValue;

	UPROPERTY(EditAnywhere, DisplayName = "점프대 트리거 종류(현재 접촉만 동작)")
	EJumpPadType JumpPadType;

	UPROPERTY(EditAnywhere, DisplayName = "트리거시 기존 속도 초기화")
	uint8 bResetVelocity : 1;
};
