#include "XR_Project_Team10/Interaction/JumpPad.h"

#include "XR_Project_Team10/Util/PPConstructorHelper.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
	RootComponent = BaseMesh;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));

	bResetVelocity = false;
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void AJumpPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("jump"));

		auto CharacterMesh = PlayableCharacter->GetMeshComp();
		FVector JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);
		if(bResetVelocity)
		{
			CharacterMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		}
		switch (JumpPadType)
		{
		case EJumpPadType::E_Contact:
			UE_LOG(LogTemp, Log, TEXT("contact jump"));

			/*GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (FPPTimerHelper::IsDelayElapsed(JumpDelayTimerHandle, JumpDelayTime))
					{
						bIsJumping = false;
						GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
					}
				}), 0.01f, true);*/
			JumpVelocityVector += CharacterMesh->GetPhysicsLinearVelocity();
			CharacterMesh->SetPhysicsLinearVelocity(JumpVelocityVector);
			break;
		case EJumpPadType::E_Timing:
			break;
		case EJumpPadType::E_Delay:
			break;
		case EJumpPadType::E_PileDriver:
			break;
		default:
			checkNoEntry();
		}
	}
}

void AJumpPad::NotifyActorEndOverlap(AActor* OtherActor)
{
	
}