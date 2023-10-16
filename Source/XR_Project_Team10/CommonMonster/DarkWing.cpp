
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
			TargetLocation.Z = 0;

			for (uint32 i = 0; i < FeatherNum; i++) {

				FVector MuzzleLocation;
				FRotator MuzzleRotation;

				GetMesh()->GetSocketWorldLocationAndRotation(FeatherSockets[i], MuzzleLocation, MuzzleRotation);

				ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if (Feather) {
					Feather->SetVariables(FeatherPower, FeatherSpeed, FeatherDeleteTime);

					FVector LaunchDirection = ((TargetLocation) - MuzzleLocation).GetSafeNormal();
					TargetLocation.Z -= LaunchDirection.Z * AttackRange;
					LaunchDirection = ((TargetLocation)-MuzzleLocation).GetSafeNormal();


					Feather->FireInDirection(LaunchDirection);
				}
			}
		}
	}
}
