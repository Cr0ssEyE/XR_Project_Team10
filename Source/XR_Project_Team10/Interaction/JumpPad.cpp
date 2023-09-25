#include "XR_Project_Team10/Interaction/JumpPad.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void AJumpPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto playableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != playableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("jump"));

		auto CharacterMesh = playableCharacter->GetMeshComp();
		FVector JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);

		switch (JumpPadType)
		{
		case E_Contact: {
			UE_LOG(LogTemp, Log, TEXT("contact jump"));

			/*GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					if (FPPTimerHelper::IsDelayElapsed(JumpDelayTimerHandle, JumpDelayTime))
					{
						bIsJumping = false;
						GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
					}
				}), 0.01f, true);*/

			CharacterMesh->SetPhysicsLinearVelocity(CharacterMesh->GetPhysicsLinearVelocity() + JumpVelocityVector * 100);
		}
			break;
		case E_Timing:
			break;
		case E_Delay:
			break;
		case E_PileDriver:
			break;
		default:
			break;
		}
	}
}

void AJumpPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}