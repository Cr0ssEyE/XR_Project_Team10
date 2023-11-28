#include "XR_Project_Team10/Interaction/Spring.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ASpring::ASpring()
{
	PrimaryActorTick.bCanEverTick = true;

	EndTopPos = EndPosition;
	EndTopPos.Z += ZValue;

	SpringAnim = FPPConstructorHelper::FindAndGetObject<UAnimSequence>(TEXT("/Script/Engine.AnimSequence'/Game/1-Graphic-Resource/Props/WithGimmick/KnockBack_Spring/AS_Spring_Bounce'"));

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetSkeletalMesh(FPPConstructorHelper::FindAndGetObject<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/1-Graphic-Resource/Props/WithGimmick/KnockBack_Spring/SKM_Spring'")));
	RootComponent = BaseMesh;
	BaseMesh->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	BaseMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(20.f, 20.f, 10.f));
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
}

void ASpring::BeginPlay()
{
	Super::BeginPlay();
}

void ASpring::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != PlayableCharacter 
		&& !GetWorldTimerManager().IsTimerActive(MoveToTimerHandle)) {
		ControlPoint = 0;
		Player = PlayableCharacter;
		StartPosition = Player->GetTruePlayerLocation()->GetActorLocation();
		StartTopPos = StartPosition;
		StartTopPos.Z += ZValue;

		BaseMesh->PlayAnimation(SpringAnim, false);
		GetWorldTimerManager().SetTimer(MoveToTimerHandle, this, &ASpring::MoveToPoint, TimerTick, true);
	}
}

void ASpring::NotifyActorEndOverlap(AActor* OtherActor)
{
}

FVector ASpring::CurvePoints(float Control)
{
	float u = 1 - Control;
	FVector Result = FMath::Pow(u, 3) * StartPosition
		+ FMath::Pow(u, 2) * Control * StartTopPos
		+ u * Control * Control * EndTopPos
		+ Control * Control * Control * EndPosition;

	return Result;
}

void ASpring::MoveToPoint()
{
	if (ControlPoint >= 1) {
		Player = nullptr;
		GetWorldTimerManager().ClearTimer(MoveToTimerHandle);
		return;
	}

	ControlPoint += TimerTick / Time;
	Player->SetActorLocation(CurvePoints(ControlPoint));
}

