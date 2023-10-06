#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, DisplayName = "바운드 높이 값")
	float BoundHeight;
	
	UPROPERTY(EditAnywhere, DisplayName = "고정 방향 벡터")
	FVector TrapVector;

	UPROPERTY(EditAnywhere, DisplayName = "고정 방향 벡터 사용(기본 설정값: 충돌 방향 반대 방향)")
	uint8 bUseTrapVector : 1;
};
