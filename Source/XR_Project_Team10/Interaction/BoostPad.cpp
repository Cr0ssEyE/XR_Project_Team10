#include "XR_Project_Team10/Interaction/BoostPad.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ABoostPad::ABoostPad()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/1-Graphic-Resource/Props/WithGimmick/Boost_Pad_Final/Jump_2.Jump_2'")));
	BaseMesh->SetCollisionProfileName(CP_GIMMICK);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = BaseMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void ABoostPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != PlayableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("boost"));
		auto CharacterMesh = PlayableCharacter->GetMeshComp();

		FVector BoostVector = CharacterMesh->GetPhysicsLinearVelocity() * AccelarationValue;
		BoostVector.Z = 0;
		PlayableCharacter->SetAccelerate();
		CharacterMesh->SetPhysicsLinearVelocity(BoostVector);
	}
}

void ABoostPad::NotifyActorEndOverlap(AActor* OtherActor)
{
}
