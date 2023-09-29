#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UENUM()
enum EJumpPadType : uint8 {
	E_Contact,	//�����ϸ� ����
	E_Timing,		//Ÿ�ֿ̹� �°� ���� ������ �� ū ����
	E_Delay,		//���� �ð� �� ����
	E_PileDriver	//���
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
