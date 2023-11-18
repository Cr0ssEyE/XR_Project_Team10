
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_FeatherProjectile.generated.h"

UCLASS()
class XR_PROJECT_TEAM10_API ADW_FeatherProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ADW_FeatherProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetVariables(uint32 Damage, float Speed, float DeleteTime);

	void FireInDirection(const FVector& Direction);
	void DestroyProjectile();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	TObjectPtr<class USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ProjectileEffectComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> HitEffectComponent;
	
	FTimerHandle DeleteTimerHandle;

private:
	UPROPERTY()
	uint32 FeatherPower;

	UPROPERTY()
	float FeatherSpeed = 100;

	UPROPERTY()
	float FeatherDeleteTime;

};
