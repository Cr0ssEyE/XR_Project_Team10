
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

		if (AttackOffsets.Num() == 0) {
			AttackOffsets.Add(FVector(0, 0, 0));
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

			//Rotate 제대로 안불러와짐
			FVector ActorOffset = this->GetActorLocation();
			FRotator ActorRotate = this->GetActorRotation();

			FVector TargetLocation = Target->GetActorLocation();
			TargetLocation.Z = 0;

			for (uint32 i = 0; i < FeatherNum; i++) {
				FVector AttackLocation = AttackOffsets[i];
				/*float Value = (ActorRotate.Vector().Z / 180) * AttackOffsets[i].X;
				AttackLocation.X = FMath::Clamp(Value, -AttackOffsets[i].X, AttackOffsets[i].X);
				AttackLocation.Y = -FMath::Clamp(Value, -AttackOffsets[i].X, AttackOffsets[i].X);
				UE_LOG(LogTemp, Log, TEXT("Angle : %f, Value : %f, X : %f Y : %f"), ActorRotate.Vector().Z, Value, AttackLocation.X, AttackLocation.Y);*/

				FVector MuzzleLocation = AttackLocation + ActorOffset;
				FRotator MuzzleRotation;

				ADW_FeatherProjectile* Feather = World->SpawnActor<ADW_FeatherProjectile>(FeatherClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if (Feather) {
					Feather->SetVariables(FeatherPower, FeatherSpeed, FeatherDeleteTime);

					FVector LaunchDirection = ((TargetLocation) - MuzzleLocation).GetSafeNormal();
					TargetLocation.Z -= LaunchDirection.Z * AttackRange;
					LaunchDirection = ((TargetLocation)-MuzzleLocation).GetSafeNormal();

					/*UE_LOG(LogTemp, Log, TEXT("%lf %lf %lf"),
						(LaunchDirection.X * AttackRange),
						(LaunchDirection.Y * AttackRange),
						(LaunchDirection.Z * AttackRange));*/

					Feather->FireInDirection(LaunchDirection);
				}
			}
		}
	}
}
