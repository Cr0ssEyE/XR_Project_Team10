#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FractureBlock.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API AFractureBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	AFractureBlock();

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<class UStaticMeshComponent> FractureMesh;
};
