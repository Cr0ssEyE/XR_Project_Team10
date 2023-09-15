// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Player/KWPlayerDataAsset.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AKWPlayerCharacter::AKWPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetMesh()->SetSkeletalMesh(FPPConstructorHelper::FindAndGetObject<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'")));
	
	GetMesh()->SetAnimClass(FPPConstructorHelper::FindAndGetClass<UAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'")));
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -80.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCapsuleSize(70.f, 70.f);
	
	RollingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RollingMesh->SetSimulatePhysics(true);
	
	PlayerComponent = Cast<UCapsuleComponent>(RootComponent);
	RootComponent = RollingMesh;
	PlayerComponent->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetUsingAbsoluteRotation(false);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	
	CharacterData = FPPConstructorHelper::FindAndGetObject<UKWPlayerDataAsset>(TEXT("/Script/XR_Project_Team10.KWPlayerDataAsset'/Game/3-CharacterTest/PlayerDataAsset.PlayerDataAsset'"), EAssertionLevel::Check);

	InputMappingContext= CharacterData->PlayerInputMappingContext;

	ToggleTypeAction = CharacterData->ToggleTypeAction;
	MoveInputAction = CharacterData->MoveInputAction;
	JumpAction = CharacterData->JumpAction;
	DA_Action = CharacterData->DA_Action;
	
	AddMoveForceValue = CharacterData->AddMoveForceValue;
	AddJumpForceValue = CharacterData->AddJumpForceValue;
	MaxMoveForceValue = CharacterData->MaxMoveForceValue;
	JustTimingTime = CharacterData->JustTimingValue;
	JumpDelayTime = CharacterData->JumpDelayTime;
	
	DA_AddForceValue = CharacterData->DA_AddForceValue;
	DA_DurationTime = CharacterData->DA_DurationTime;
	DA_CoolDownTime = CharacterData->DA_CoolDownTime;
}

// Called when the game starts or when spawned
void AKWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	JumpDelayRemainTime = 0.0f;
	RollingMesh->SetMassOverrideInKg(NAME_None, 50.f);
	RollingMesh->SetStaticMesh(nullptr);
	RollingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsRolling = false;
	bIsJumping = false;
	SpringArm->SetRelativeLocation(PlayerComponent->GetRelativeLocation());
	PlayerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void AKWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// SpringArm->SetRelativeLocation(RootComponent->GetRelativeLocation());
	
	if(PlayerComponent->GetRelativeScale3D() != FVector::ZeroVector)
	{
		SpringArm->SetRelativeLocation(PlayerComponent->GetComponentToWorld().GetLocation());
	}
	else
	{	
		SpringArm->SetRelativeLocation(RootComponent->GetComponentToWorld().GetLocation());
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f %f %f"), RollingMesh->GetComponentVelocity().X, RollingMesh->GetComponentVelocity().Y, RollingMesh->GetComponentVelocity().Z));
}

// Called to bind functionality to input
void AKWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(InputMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ToggleTypeAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::ToggleCharacterTypeAction);

	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AKWPlayerCharacter::MoveAction);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::JumpAddForceAction);
	
	EnhancedInputComponent->BindAction(DA_Action, ETriggerEvent::Started, this, &AKWPlayerCharacter::DA_AddForceAction);
}

void AKWPlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	
	if(bIsRolling)
	{
		FVector AddForceResult = MoveDirection * AddMoveForceValue;
		if(GetVelocity().X >= MaxMoveForceValue && AddForceResult.X > 0)
		{
			AddForceResult.X = 0.0f;
		}
		if(GetVelocity().X <= -MaxMoveForceValue && AddForceResult.X <= 0)
		{
			AddForceResult.X = 0.0f;
		}
		if(GetVelocity().Y >= MaxMoveForceValue && AddForceResult.Y > 0)
		{
			AddForceResult.Y = 0.0f;
		}
		if(GetVelocity().Y <= -MaxMoveForceValue && AddForceResult.Y <= 0)
		{
			AddForceResult.Y = 0.0f;
		}
		RollingMesh->AddForce(AddForceResult * 10000);
	}
	else
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
		AddMovementInput(MoveDirection, MovementVectorSize);
	}
}

void AKWPlayerCharacter::JumpAddForceAction(const FInputActionValue& Value)
{
	if(!GetWorldTimerManager().IsTimerActive(JumpDelayTimerHandle))
	{
		JumpDelayRemainTime = 0.0f;
		GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			JumpDelayRemainTime += GetWorld()->DeltaTimeSeconds;

			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
		
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(70.0f),
			Params);

			if(bResult)
			{
				bIsJumping = false;
				GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
			}
		}), 0.01f, true);
		bIsJumping = true;
		const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
		const FVector Movement = Direction * AddJumpForceValue;
		RollingMesh->AddForce(Movement * -GetWorld()->GetGravityZ() * RollingMesh->GetMass() * 1.5f);
	}
}

void AKWPlayerCharacter::ToggleCharacterTypeAction(const FInputActionValue& Value)
{
	if(bIsRolling)
	{
		bIsRolling = false;
		RollingMesh->SetStaticMesh(nullptr);
		RollingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RollingMesh->SetRelativeRotation(FRotator::ZeroRotator);
		
		PlayerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerComponent->SetWorldLocation(RollingMesh->GetComponentToWorld().GetLocation());
		PlayerComponent->SetRelativeScale3D(FVector::OneVector);
	}
	else
	{
		bIsRolling = true;
		RollingMesh->SetStaticMesh(CharacterData->PlayerMesh);
		RollingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RollingMesh->SetWorldLocation(PlayerComponent->GetComponentToWorld().GetLocation());
		
		PlayerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerComponent->SetRelativeScale3D(FVector::ZeroVector);
		VelocityDecelerateTimer();
	}
}

void AKWPlayerCharacter::RB_JustTimingAction(const FInputActionValue& Value)
{
	// 리바운드 구현 후 개편 예정
	/*
	JustTimingElapsedTime = 0;
	if(!bIsJumping)
	{
		return;
	}
	GetWorldTimerManager().SetTimer(JustTimingTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			JustTimingElapsedTime += GetWorld()->DeltaTimeSeconds;
		
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
		
			bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(70.0f),
			Params);
			DrawDebugSphere(GetWorld(), GetActorLocation(), 70.f, 32, FColor::Red);
		
			if(bResult)
			{
				// AD_AddForceAction();
				bIsJumping = false;
				GetWorldTimerManager().ClearTimer(JustTimingTimerHandle);
				GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
			}
		
			if(JustTimingElapsedTime >= JustTimingTime)
			{
				JustTimingElapsedTime = 0;
				bIsJumping = false;
				GetWorldTimerManager().ClearTimer(JustTimingTimerHandle);
				GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
			}
		}), 0.01f, true);
	*/
}

void AKWPlayerCharacter::DA_AddForceAction(const FInputActionValue& Value)
{
	if(GetWorldTimerManager().IsTimerActive(DA_CoolDownTimerHandle))
	{
		return;
	}
	
	// AddMoveForceValue += BoostAddForceValue;
	// MaxMoveForceValue += BoostAddMaxForceValue;
	const FVector DirectionX = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	const FVector DirectionY = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	const FVector MovementX = DirectionX * DA_AddForceValue;
	const FVector MovementY = DirectionY * DA_AddForceValue;
	RollingMesh->AddForce(MovementX * 1000000 * RollingMesh->GetComponentVelocity().GetSafeNormal().X);
	RollingMesh->AddForce(MovementY * 1000000 * RollingMesh->GetComponentVelocity().GetSafeNormal().Y);
	
	GetWorldTimerManager().SetTimer(DA_DurationTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(DA_DurationTimerHandle, DA_DurationTime))
			{
				DA_CoolDownTimer();
				GetWorldTimerManager().ClearTimer(DA_DurationTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(DA_DurationTimerHandle);
			}
		}), 0.001f, true);
}

void AKWPlayerCharacter::DA_CoolDownTimer()
{
	GetWorldTimerManager().SetTimer(DA_CoolDownTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(DA_CoolDownTimerHandle, DA_CoolDownTime))
			{
				DA_CoolDownRemainTime = 0;
				GetWorldTimerManager().ClearTimer(DA_CoolDownTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(DA_CoolDownTimerHandle);
			}
		}), 0.001f, true);
}

void AKWPlayerCharacter::VelocityDecelerateTimer()
{
	VelocityDecelerateTime = 0.1f;
	GetWorldTimerManager().SetTimer(VelocityDecelerationTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(VelocityDecelerationTimerHandle, VelocityDecelerateTime))
			{
				const FVector DecelerateVelocity = GetVelocity().GetSafeNormal();
				RollingMesh->AddForce(DecelerateVelocity * -10000);
			}
			if(!bIsRolling)
			{
				GetWorldTimerManager().ClearTimer(VelocityDecelerationTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(VelocityDecelerationTimerHandle);
			}
		}), 0.001f, true);
}


