#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "JumpPad.generated.h"

UENUM()
enum class EJumpPadType : uint8
{
	E_Contact UMETA(DisplayName = "접촉"),//접촉하면 점프
	E_Timing UMETA(DisplayName = "타이밍"),	//파일드라이버 사용시 더 큰 점프
	E_Delay UMETA(DisplayName = "지연"),	//일정 시간 후 점프
	E_PileDriver UMETA(DisplayName = "파일 드라이버")	//파일드라이버 사용할 때만 점프
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
	void ContactJump();
	void DelayJump();
	void TimingJump();
	void PileDriverJump();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimSequence> JumpAnim;
	
	UPROPERTY(EditAnywhere, DisplayName = "점프 크기값")
	float JumpForceValue;

	UPROPERTY(EditAnywhere, DisplayName = "점프대 트리거 종류")
	EJumpPadType JumpPadType;

	UPROPERTY(EditAnywhere, DisplayName = "트리거시 기존 속도 초기화")
	uint8 bResetVelocity : 1;

	UPROPERTY(EditAnywhere, DisplayName = "[지연] 지연 시간")
	float DelayTime;

	UPROPERTY(EditAnywhere, DisplayName = "[타이밍] 점프 추가값")
	float JumpAddForceValue;

	UPROPERTY()
	uint8 bIsPlayerIn : 1;

	UPROPERTY()
	FVector JumpVelocityVector;

	UPROPERTY()
	TObjectPtr<AKWPlayerCharacter> Player;


	FTimerHandle JumpTimerHandle;
};
