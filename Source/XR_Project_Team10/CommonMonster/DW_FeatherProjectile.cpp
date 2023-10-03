#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
ADW_FeatherProjectile::ADW_FeatherProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<class USphereComponent>(TEXT("SphereComponent"));
	// 콜리전 반경
	CollisionComponent->InitSphereRadius(15.0f);
	// 루트 컴포넌트를 콜리전 컴포넌트로 설정
	RootComponent = CollisionComponent;

	// 발사체 운동
	ProjectileMovementComponent = CreateDefaultSubobject<class UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = FeatherSpeed;
	ProjectileMovementComponent->MaxSpeed = FeatherSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.0f;

}

void ADW_FeatherProjectile::BeginPlay()
{
	Super::BeginPlay();
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void ADW_FeatherProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ProjectileMovementComponent->Velocity = FVector(0, 0, 0);

	auto playableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != playableCharacter) {
		UE_LOG(LogTemp, Log, TEXT("player hit"));
	}
	else {
		//작동 안함
		UE_LOG(LogTemp, Log, TEXT("another hit"));
	}
}

void ADW_FeatherProjectile::NotifyActorEndOverlap(AActor* OtherActor)
{
}

void ADW_FeatherProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADW_FeatherProjectile::FireInDirection(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}


// 피격되지 않으면 땅에 박히고 2초 뒤 사라짐