#include "XR_Project_Team10/Interaction/TrapPad.h"

ATrapPad::ATrapPad()
{
 	PrimaryActorTick.bCanEverTick = true;

}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void ATrapPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != PlayableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("trap"));

		auto CharacterMesh = PlayableCharacter->GetMeshComp();

		CharacterMesh->SetPhysicsLinearVelocity(CharacterMesh->GetPhysicsLinearVelocity() + TrapVector * 100);

	}
}

void ATrapPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}
