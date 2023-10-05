#include "XR_Project_Team10/Interaction/BoostPad.h"

ABoostPad::ABoostPad()
{
	PrimaryActorTick.bCanEverTick = true;

}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void ABoostPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != PlayableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("boost"));
		auto CharacterMesh = PlayableCharacter->GetMeshComp();

		CharacterMesh->SetPhysicsLinearVelocity(CharacterMesh->GetPhysicsLinearVelocity() * AccelarationValue);
	}
}

void ABoostPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}
