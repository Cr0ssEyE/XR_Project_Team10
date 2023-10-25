
#include "XR_Project_Team10/CommonMonster/DarkWing.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"

ADarkWing::ADarkWing()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/DarkWing"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> Feather(TEXT("/Game/10-Common-Monster-Base/MyDW_FeatherProjectile"));
		if (Feather.Succeeded()) {
			FeatherClass = Feather.Object->GeneratedClass;
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
void ADarkWing::AttackOmen(AActor* Target)
{
	// 전조
	UE_LOG(LogTemp, Log, TEXT("DarkWing Attack Omen"));
}

//공격
void ADarkWing::Attack(AActor* Target)
{
	// 플레이어를 향해 깃털 n개 발사
	if (nullptr != FeatherClass) {

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FVector TargetLocation = Target->GetActorLocation();
			FVector TargetLocationVector;

			FVector MuzzleLocation;
			FRotator MuzzleRotation;
			FVector LeftSocket, RightSocket;

			GetMesh()->GetSocketWorldLocationAndRotation(FeatherSockets[0], LeftSocket, MuzzleRotation);
			GetMesh()->GetSocketWorldLocationAndRotation(FeatherSockets[1], RightSocket, MuzzleRotation);

			for (uint32 i = 1; i < FeatherNum + 1; i++) {
				MuzzleLocation = FVector::SlerpVectorToDirection(LeftSocket, RightSocket, i / (double)(FeatherNum + 1));

				ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if (Feather) {
					Feather->SetVariables(FeatherPower, FeatherSpeed, FeatherDeleteTime);


					TargetLocationVector = (TargetLocation - MuzzleLocation).GetSafeNormal() * AttackRange;
					float Z = TargetLocationVector.Z;
					TargetLocationVector = TargetLocation;
					TargetLocationVector.Z -= Z;
					TargetLocationVector = (TargetLocationVector - MuzzleLocation).GetSafeNormal();

					//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), TargetLocation.X - MuzzleLocation.X, TargetLocation.Y - MuzzleLocation.Y, TargetLocation.Z - MuzzleLocation.Z);
					//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), TargetLocationVector.X, TargetLocationVector.Y, TargetLocationVector.Z);

					FVector LaunchDirection = TargetLocationVector;


					Feather->FireInDirection(LaunchDirection);
				}
			}

			OnAttackFinished.ExecuteIfBound();
		}
	}
}
