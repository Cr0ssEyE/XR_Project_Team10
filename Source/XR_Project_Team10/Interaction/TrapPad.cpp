#include "XR_Project_Team10/Interaction/TrapPad.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ATrapPad::ATrapPad()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	BoundHeight = 100.f;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
	RootComponent = BaseMesh;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	
	bUseTrapVector = false;
}

void ATrapPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("trapOverlapBegin"));

		auto CharacterMesh = PlayableCharacter->GetMeshComp();

		if(bUseTrapVector)
		{
			PlayableCharacter->RB_ApplyReBoundByObjectType(CharacterMesh->GetPhysicsLinearVelocity() + TrapVector * 100, EReBoundObjectType::Gimmick);
		}
		else
		{
			FVector ReBoundVector = CharacterMesh->GetPhysicsLinearVelocity() + FVector(0.f, 0.f, -BoundHeight);
			PlayableCharacter->RB_ApplyReBoundByObjectType(ReBoundVector, EReBoundObjectType::Gimmick);
		}
	}
}

void ATrapPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}
