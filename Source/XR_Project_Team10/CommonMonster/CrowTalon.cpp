#include "XR_Project_Team10/CommonMonster/CrowTalon.h"
#include "XR_Project_Team10/CommonMonster/CommonMonsterDataAsset.h"
#include "UObject/ConstructorHelpers.h"

ACrowTalon::ACrowTalon()
{static ConstructorHelpers::FObjectFinder<UDataAsset> DataAsset(TEXT("/Game/Rolling-Kiwi/Datas/DataAssets/CrowTalon"));
	if (DataAsset.Succeeded()) {
		UDataAsset* dataAsset = DataAsset.Object;
		MonsterData = Cast<UCommonMonsterDataAsset>(dataAsset);
	}
}

void ACrowTalon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrowTalon::AttackOmen(AActor* Target)
{
	UE_LOG(LogTemp, Log, TEXT("CrowTalon Attack Omen"));
}

void ACrowTalon::Attack(AActor* Target)
{

}

void ACrowTalon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

