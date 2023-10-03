#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
ADW_FeatherProjectile::ADW_FeatherProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<class USphereComponent>(TEXT("SphereComponent"));
	// �ݸ��� �ݰ�
	CollisionComponent->InitSphereRadius(15.0f);
	// ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� ����
	RootComponent = CollisionComponent;

	// �߻�ü �
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
		//�۵� ����
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


// �ǰݵ��� ������ ���� ������ 2�� �� �����