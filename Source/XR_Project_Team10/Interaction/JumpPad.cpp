#include "XR_Project_Team10/Interaction/JumpPad.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
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
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
	bResetVelocity = false;
	bIsPlayerIn = false;

	JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void AJumpPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = true;
		PlayerInTime = 0.0f;

		PlayerMeshComp = PlayableCharacter->GetMeshComp();
		
		if(bResetVelocity)
		{
			PlayerMeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
		}
		switch (JumpPadType)
		{
		case EJumpPadType::E_Contact:
			UE_LOG(LogTemp, Log, TEXT("contact jump"));
			ContactJump();
			break;
		case EJumpPadType::E_Timing:
			UE_LOG(LogTemp, Log, TEXT("timing jump"));
			TimingJump();
			break;
		case EJumpPadType::E_Delay:
			UE_LOG(LogTemp, Log, TEXT("delay jump"));
			GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &AJumpPad::DelayJump, DelayTime, false);
			break;
		case EJumpPadType::E_PileDriver:
			UE_LOG(LogTemp, Log, TEXT("piledriver jump"));
			PileDriverJump();
			break;
		default:
			checkNoEntry();
		}
	}
}

void AJumpPad::ContactJump()
{
	if (nullptr != PlayerMeshComp) {
		JumpVelocityVector += PlayerMeshComp->GetPhysicsLinearVelocity();
		PlayerMeshComp->SetPhysicsLinearVelocity(JumpVelocityVector);
	}
}

void AJumpPad::DelayJump() {
	ContactJump();
}

void AJumpPad::TimingJump() {
	if (true) {
		JumpVelocityVector.Z += JumpAddForceValue;
	}
	ContactJump();
	JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);
}

void AJumpPad::PileDriverJump()
{
	if (true) {

	}
}

void AJumpPad::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = false;
		PlayerMeshComp = nullptr;
	}
}