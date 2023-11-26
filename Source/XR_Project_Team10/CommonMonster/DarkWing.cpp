
#include "XR_Project_Team10/CommonMonster/DarkWing.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

ADarkWing::ADarkWing()
{
	MonsterData = FPPConstructorHelper::FindAndGetObject<UCommonMonsterDataAsset>(TEXT("/Script/XR_Project_Team10.CommonMonsterDataAsset'/Game/Rolling-Kiwi/Datas/DataAssets/DarkWingData.DarkWingData'"), EAssertionLevel::Check);

	FeatherClass = FPPConstructorHelper::FindAndGetClass<ADW_FeatherProjectile>(TEXT("/Script/Engine.Blueprint'/Game/9-CommonAI/AI/MyDW_FeatherProjectile.MyDW_FeatherProjectile_C'"), EAssertionLevel::Check);

	DeadMontage = FPPConstructorHelper::FindAndGetObject<UAnimMontage>(TEXT("/Script/Engine.AnimMontage'/Game/1-Graphic-Resource/Monster/DarkWing/Animation/AM_DarkWing_Dead.AM_DarkWing_Dead'"), EAssertionLevel::Check); 
	HitMontage = FPPConstructorHelper::FindAndGetObject<UAnimMontage>(TEXT("/Script/Engine.AnimMontage'/Game/1-Graphic-Resource/Monster/DarkWing/Animation/AM_DarkWing_Hit.AM_DarkWing_Hit'"));

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

						FVector LaunchDirection = TargetLocationVector;

						Feather->FireInDirection(LaunchDirection);
					}
				}
			}
		}
	}
}

void ADarkWing::PlayDeadAnimation()
{
	Super::PlayDeadAnimation();
}

void ADarkWing::PlayHitAnimation()
{
	Super::PlayHitAnimation();
}
