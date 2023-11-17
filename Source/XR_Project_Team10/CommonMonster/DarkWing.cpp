
#include "XR_Project_Team10/CommonMonster/DarkWing.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

ADarkWing::ADarkWing()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/DarkWingData"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Feather(TEXT("/Game/10-Common-Monster-Base/MyDW_FeatherProjectile"));
		if (Feather.Succeeded()) {
			FeatherClass = Feather.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/1-Graphic-Resource/Monster/Monster_1/Dead/NormalMonster1_Dead"));
	if (DeadMontageRef.Object) {
		DeadMontage = DeadMontageRef.Object;
	}
}

void ADarkWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADarkWing::BeginPlay()
{
	Super::BeginPlay();
}

//공격 전조
void ADarkWing::AttackOmen()
{
	Super::AttackOmen();
	// 전조
	UE_LOG(LogTemp, Log, TEXT("DarkWing Attack Omen"));
	DetectTarget = Cast<AKWLocationDetector>(PlayerTarget);
	TargetLocation = DetectTarget->GetActorLocation();
}

//공격
void ADarkWing::Attack()
{
	float TurnSpeed = 5.f;
	FVector LookVector = PlayerTarget->GetActorLocation() - GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	Super::Attack();
}

void ADarkWing::AttackEnd()
{
	Super::AttackEnd();
	PlayerTarget = nullptr;
}

void ADarkWing::AttackBehaviour()
{
	UE_LOG(LogTemp, Log, TEXT("DarkWing Attack"));
	// 플레이어를 향해 깃털 n개 발사
	if (nullptr != FeatherClass) {
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			if (DetectTarget) {
				FVector TargetLocationVector;

				FVector MuzzleLocation;
				FRotator MuzzleRotation;
				FVector LeftSocket, RightSocket;

				GetMesh()->GetSocketWorldLocationAndRotation(FeatherSockets[0], LeftSocket, MuzzleRotation);
				GetMesh()->GetSocketWorldLocationAndRotation(FeatherSockets[1], RightSocket, MuzzleRotation);
				for (uint32 i = 1; i < FeatherNum + 1; i++) {
					double Alpha = i / (double)(FeatherNum + 1);
					MuzzleLocation = FMath::Lerp(LeftSocket, RightSocket, FVector(Alpha, Alpha, Alpha));
					MuzzleLocation.Z = this->GetActorLocation().Z;

					ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);

					if (Feather) {
						Feather->SetVariables(FeatherPower, FeatherSpeed, FeatherDeleteTime);

						TargetLocationVector = MuzzleLocation + ((TargetLocation - MuzzleLocation) * AttackRange);
						TargetLocationVector.Z = 0;
						TargetLocationVector = (TargetLocationVector - MuzzleLocation).GetSafeNormal();

						//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), TargetLocationVector.X, TargetLocationVector.Y, TargetLocationVector.Z);
						//UE_LOG(LogTemp, Log, TEXT(""));

						FVector LaunchDirection = TargetLocationVector;

						Feather->FireInDirection(LaunchDirection);
					}
				}
			}
		}
	}
}
