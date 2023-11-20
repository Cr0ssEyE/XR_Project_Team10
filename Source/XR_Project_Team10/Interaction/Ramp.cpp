// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Interaction/Ramp.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "Ramp.h"

// Sets default values
ARamp::ARamp()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Game/1-Graphic-Resource/Props/JumpSlide/SM_JumpSlide")));
	RootComponent = BaseMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetWorldLocation(FVector(0, 0, 27.f));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(20.f, 20.f, 22.f));
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);

	CheckZ = CollisionBox->GetScaledBoxExtent().Z * JumpTimingZ;
}

// Called when the game starts or when spawned
void ARamp::BeginPlay()
{
	Super::BeginPlay();
}

void ARamp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);
	if (PlayableCharacter) {
		StartZ = PlayableCharacter->GetTruePlayerLocation()->GetActorLocation().Z;

		auto CharacterMesh = PlayableCharacter->GetMeshComp();

		CharacterMesh->SetPhysicsLinearVelocity(CharacterMesh->GetPhysicsLinearVelocity() * (1 - DecelerationValue));
	}
}

void ARamp::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);
	if (PlayableCharacter) {
		if (PlayableCharacter->GetTruePlayerLocation()->GetActorLocation().Z - StartZ > CheckZ) {
			auto PlayerMeshComp = PlayableCharacter->GetMeshComp();

			FVector JumpVelocityVector = FVector(0.f, 0.f, JumpValue);
			PlayerMeshComp->SetPhysicsLinearVelocity(JumpVelocityVector);
		}
	}
}


