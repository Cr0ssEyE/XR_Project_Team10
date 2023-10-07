#include "XR_Project_Team10/Interaction/TrapPad.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

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

	InteractionCoolDown = 1.f;
	bUseTrapVector = false;
}

void ATrapPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(GetWorldTimerManager().IsTimerActive(DisableInteractionTimerHandle))
	{
		UE_LOG(LogTemp, Log, TEXT("상호작용 쿨타임"));
		return;
	}
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
		ActiveInteractionCoolDown();
	}
}

void ATrapPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}

void ATrapPad::ActiveInteractionCoolDown()
{
	GetWorldTimerManager().SetTimer(DisableInteractionTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(DisableInteractionTimerHandle, InteractionCoolDown))
		{
			GetWorldTimerManager().ClearTimer(DisableInteractionTimerHandle);
			FPPTimerHelper::InvalidateTimerHandle(DisableInteractionTimerHandle);
		}
	}), 0.01f, true);
}
