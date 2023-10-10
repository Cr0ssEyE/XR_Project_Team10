// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Object/KWReBoundTestActor.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"

// Sets default values
AKWReBoundTestActor::AKWReBoundTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
	CollisionBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKWReBoundTestActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->InitBoxExtent(FVector(200.f, 200.f, 400.f));
	ReBoundObject = EReBoundObjectType::Gimmick;
}

void AKWReBoundTestActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AKWPlayerCharacter* Player = Cast<AKWPlayerCharacter>(OtherActor);
	if(Player)
	{
		CollisionDirection = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		CollisionDirection.Z = 1.5f;
		CollisionDirection *= 1000.f;
		Player->RB_ApplyReBoundByObjectType(CollisionDirection, ReBoundObject);
	}
}

// Called every frame
void AKWReBoundTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

