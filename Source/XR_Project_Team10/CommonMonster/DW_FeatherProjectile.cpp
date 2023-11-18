#include "XR_Project_Team10/CommonMonster/DW_FeatherProjectile.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

#include "XR_Project_Team10/CommonMonster/DarkWing.h"
#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"

ADW_FeatherProjectile::ADW_FeatherProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<class USphereComponent>(TEXT("SphereComponent"));
	// 콜리전 반경
	CollisionComponent->InitSphereRadius(15.0f);
	// 루트 컴포넌트를 콜리전 컴포넌트로 설정
	RootComponent = CollisionComponent;

	ProjectileEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectlieEffect"));
	ProjectileEffectComponent->SetupAttachment(RootComponent);
	
	HitEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitEffect"));
	HitEffectComponent->SetupAttachment(RootComponent);
	
	//// 발사체 운동
	ProjectileMovementComponent = CreateDefaultSubobject<class UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 0;
	ProjectileMovementComponent->MaxSpeed = 0;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.0f;

}

void ADW_FeatherProjectile::BeginPlay()
{
	Super::BeginPlay();
	HitEffectComponent->Deactivate();
}

void ADW_FeatherProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{

	auto playableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (nullptr != playableCharacter)
	{
		ProjectileMovementComponent->Velocity = FVector(0, 0, 0);
		FDamageEvent DamageEvent;
		playableCharacter->TakeDamage(FeatherPower, DamageEvent, nullptr, this);
		HitEffectComponent->Activate();
		this->Destroy();
		UE_LOG(LogTemp, Log, TEXT("player hit"));
	}
}

void ADW_FeatherProjectile::NotifyActorEndOverlap(AActor* OtherActor)
{
	
}

void ADW_FeatherProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovementComponent->Velocity = FVector(0, 0, 0);
	HitEffectComponent->Activate();
	GetWorld()->GetTimerManager().SetTimer(DeleteTimerHandle, this, &ADW_FeatherProjectile::DestroyProjectile, FeatherDeleteTime, false);
	
}

void ADW_FeatherProjectile::DestroyProjectile()
{
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(DeleteTimerHandle);
}

void ADW_FeatherProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADW_FeatherProjectile::SetVariables(uint32 Damage, float Speed, float DeleteTime)
{
	FeatherPower = Damage;
	FeatherSpeed = Speed;
	FeatherDeleteTime = DeleteTime;

	ProjectileMovementComponent->InitialSpeed = FeatherSpeed;
	ProjectileMovementComponent->MaxSpeed = FeatherSpeed;
}

void ADW_FeatherProjectile::FireInDirection(const FVector& Direction)
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

