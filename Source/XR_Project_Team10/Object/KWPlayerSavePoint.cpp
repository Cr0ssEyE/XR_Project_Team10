// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWPlayerSavePoint.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

// Sets default values
AKWPlayerSavePoint::AKWPlayerSavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(CP_GIMMICK);
	BaseMesh->SetCollisionObjectType(ECC_PLAYER_ONLY);
	RootComponent = BaseMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetBoxExtent(FVector::OneVector * 100.f);
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
	CollisionBox->SetCollisionObjectType(ECC_PLAYER_ONLY);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetupAttachment(RootComponent);

	PlayerSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocation"));
	PlayerSpawnLocation->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AKWPlayerSavePoint::BeginPlay()
{
	Super::BeginPlay();
	bIsTriggered = false;
}

// Called every frame
void AKWPlayerSavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKWPlayerSavePoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AKWLocationDetector* PlayerLocation = Cast<AKWLocationDetector>(OtherActor);
	if(PlayerLocation && !bIsTriggered)
	{
		bIsTriggered = true;
		AKWPlayerCharacter* PlayerCharacter = Cast<AKWPlayerCharacter>(PlayerLocation->GetTargetCharacter());
		if(PlayerCharacter)
		{
			//TODO: 체력 최대로 회복시켜주기
			// PlayerCharacter->RecoverMaxHp();
			RegisterSavePointDelegate.Broadcast(this);
		}
	}
}

