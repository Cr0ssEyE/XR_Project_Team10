#include "XR_Project_Team10/Interaction/Spring.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ASpring::ASpring()
{
	PrimaryActorTick.bCanEverTick = true;

	EndTopPos = EndPosition;
	EndTopPos.Z += ZValue;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
	RootComponent = BaseMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
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
		Player = PlayableCharacter;
		ControlPoint = 0;
		StartPosition = Player->GetTruePlayerLocation()->GetActorLocation();
		StartTopPos = StartPosition;
		StartTopPos.Z += ZValue;
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

