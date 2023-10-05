#include "XR_Project_Team10/Interaction/FractureBlock.h"

AFractureBlock::AFractureBlock()
{
	PrimaryActorTick.bCanEverTick = true;
	FractureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	FractureMesh->SetSimulatePhysics(true);

	
	
}

void AFractureBlock::BeginPlay()
{
	Super::BeginPlay();
	//FractureMesh->AddForce(FVector(1000, 0, 0));
}