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

	if (JumpPadType == EJumpPadType::E_Timing) {
		//GetWorldTimerManager().SetTimer(JumpTimerHandle, TimingJump, DelayTime, true);
	}
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void AJumpPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = true;
		PlayerInTime = 0.0f;

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

			JumpVelocityVector += CharacterMesh->GetPhysicsLinearVelocity();
			CharacterMesh->SetPhysicsLinearVelocity(JumpVelocityVector);
			break;
		case EJumpPadType::E_Timing:
		{
			UE_LOG(LogTemp, Log, TEXT("timing jump"));
			//TFunctionRef FuncRef = TimingJump(PlayableCharacter);
			//GetWorldTimerManager().SetTimer(JumpTimerHandle, DelayJump(PlayableCharacter), DelayTime, false);
		}
			break;
		case EJumpPadType::E_Delay:
			UE_LOG(LogTemp, Log, TEXT("delay jump"));

			break;
		case EJumpPadType::E_PileDriver:
			UE_LOG(LogTemp, Log, TEXT("piledriver jump"));

			//player 파일드라이버 사용 확인
			JumpVelocityVector += CharacterMesh->GetPhysicsLinearVelocity();
			CharacterMesh->SetPhysicsLinearVelocity(JumpVelocityVector);
			break;
		default:
			checkNoEntry();
		}
	}
}

void DelayJump(AKWPlayerCharacter* Player) {

}

void TimingJump() {
	
}

void AJumpPad::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = false;
	}
}