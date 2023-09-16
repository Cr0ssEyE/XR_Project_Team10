// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Player/KWPlayerDataAsset.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	AttackAction = CharacterData->AttackAction;

	DefaultVelocityValue = CharacterData->DefaultVelocityValue;
	DefaultMaxVelocityValue = CharacterData->DefaultMaxVelocityValue;
	VelocityMultiplyValuesByGear = CharacterData->VelocityMultiplyValuesByGear;
	MaxVelocityMagnificationByGear = CharacterData->MaxVelocityMagnificationByGear;
	AddJumpForceValue = CharacterData->AddJumpForceValue;
	JustTimingTime = CharacterData->JustTimingValue;
	JumpDelayTime = CharacterData->JumpDelayTime;
	
	DA_AddVelocityValue = CharacterData->DA_AddVelocityValue;
	DA_DurationTime = CharacterData->DA_DurationTime;
	DropDownVelocityValue = CharacterData->DropDownVelocityValue;
	DropDownMinimumHeightValue = CharacterData->DropDownMinimumHeightValue;
	AttackCoolDownTime = CharacterData->AttackCoolDownTime;
	ColorsByGear = CharacterData->ColorsByGear;
}

// Called when the game starts or when spawned
void AKWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	RollingMesh->SetMassOverrideInKg(NAME_None, 50.f);
	RollingMesh->SetStaticMesh(nullptr);
	RollingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CurrentGearState = EGearState::GearOne;
	bIsDead = false;
	bIsMoving = false;
	bIsRolling = false;
	bIsJumping = false;
	bIsAttackOnGoing = false;
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
		SpringArm->SetRelativeLocation(RootComponent->GetComponentToWorld().GetLocation());;
	}
	if(!bIsMoving)
	{
		VelocityDecelerateTimer();
	}
	if(bIsRolling)
	{
		CheckGearState();
	}
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%d"), static_cast<uint8>(CurrentGearState)));
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), RollingMesh->GetPhysicsLinearVelocity().Size2D()));
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f %f %f"), RollingMesh->GetPhysicsLinearVelocity().X, RollingMesh->GetPhysicsLinearVelocity().Y, RollingMesh->GetPhysicsLinearVelocity().Z));
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

	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &AKWPlayerCharacter::MoveActionCompleted);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::JumpAddForceAction);
	
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::AttackActionSequence);
}

void AKWPlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	bIsMoving = true;
	MoveInputValue = Value.Get<FVector2D>();
	float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}

	FVector MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.0f);
	
	if(bIsRolling)
	{
		FVector AddVelocityResult = FVector::OneVector;
		if(static_cast<uint8>(CurrentGearState) < static_cast<uint8>(EGearState::GearFour))
		{
			AddVelocityResult = MoveDirection * DefaultVelocityValue * VelocityMultiplyValuesByGear[static_cast<uint8>(CurrentGearState)];
		}
		AddVelocityResult.Z = 0.f;
		
		if(RollingMesh->GetPhysicsLinearVelocity().X >= DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)] && MoveDirection.X > 0)
		{
			AddVelocityResult.X = 0.f;
		}
		if(RollingMesh->GetPhysicsLinearVelocity().X <= DefaultMaxVelocityValue * -MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)] && MoveDirection.X < 0)
		{
			AddVelocityResult.X = 0.f;
		}
		if(RollingMesh->GetPhysicsLinearVelocity().Y >= DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)] && MoveDirection.Y > 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		if(RollingMesh->GetPhysicsLinearVelocity().Y <= DefaultMaxVelocityValue * -MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)] && MoveDirection.Y < 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		
		if(!bIsAttackOnGoing)
		{
			RollingMesh->SetPhysicsLinearVelocity(RollingMesh->GetPhysicsLinearVelocity() + AddVelocityResult);
		}
	}
	else
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
		AddMovementInput(MoveDirection, 1.f);
	}
}

void AKWPlayerCharacter::MoveActionCompleted(const FInputActionValue& Value)
{
	bIsMoving = false;
	if(bIsRolling)
	{
		VelocityDecelerateTimer();
	}
}

void AKWPlayerCharacter::JumpAddForceAction(const FInputActionValue& Value)
{
	if(bIsJumping || GetWorldTimerManager().IsTimerActive(JumpDelayTimerHandle) || RollingMesh->GetPhysicsLinearVelocity().Z > 30.f)
	{
		return;
	}
	
	GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(JumpDelayTimerHandle, JumpDelayTime))
		{
			bIsJumping = false;
			GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
		}
	}), 0.01f, true);
	bIsJumping = true;
	const FVector JumpVelocityVector = FVector(0.f, 0.f, AddJumpForceValue);
	RollingMesh->SetPhysicsLinearVelocity(RollingMesh->GetPhysicsLinearVelocity() + JumpVelocityVector);
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
		RollingMesh->SetStaticMesh(CharacterData->PlayerRollingMesh);
		RollingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RollingMesh->SetWorldLocation(PlayerComponent->GetComponentToWorld().GetLocation());
		
		PlayerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerComponent->SetRelativeScale3D(FVector::ZeroVector);
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

void AKWPlayerCharacter::AttackActionSequence(const FInputActionValue& Value)
{
	if(GetWorldTimerManager().IsTimerActive(AttackCoolDownTimerHandle))
	{
		return;
	}
	
	if(bIsJumping && abs(RollingMesh->GetPhysicsLinearVelocity().Z) > DropDownMinimumHeightValue)
	{
		GetWorldTimerManager().SetTimer(DropDownTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				RollingMesh->SetSimulatePhysics(false);
				if(FPPTimerHelper::IsDelayElapsed(DropDownTimerHandle, 0.2f))
				{
					RollingMesh->SetSimulatePhysics(true);
					const FVector DroppingVelocity = FVector(0.f, 0.f, -DropDownVelocityValue);
					RollingMesh->SetPhysicsLinearVelocity(DroppingVelocity);
					AttackCoolDownTimer();
					GetWorldTimerManager().ClearTimer(DropDownTimerHandle);
					FPPTimerHelper::InvalidateTimerHandle(DropDownTimerHandle);
				}
			}), 0.01f, true);
	}
	else if(abs(RollingMesh->GetPhysicsLinearVelocity().Z) < 20.f)
	{
		bIsAttackOnGoing = true;
		CurrentGearState = EGearState::GearThree;
		FVector2D MousePosition;
		int ScreenSizeX;
		int ScreenSizeY;
		
		const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);
		
		const FVector MousePosition3D = FVector(-MousePosition.Y, MousePosition.X, 0.f);
		const FVector ScreenCenter3D = FVector(-ScreenSizeY / 2, ScreenSizeX / 2, 0.0f);
		const FVector AD_Direction = (MousePosition3D - ScreenCenter3D).GetSafeNormal();

		VelocityDecelerateTarget = FVector(100.f, 100.f ,0.f);
		
		GetWorldTimerManager().SetTimer(DA_DurationTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				if(FPPTimerHelper::IsDelayElapsed(DA_DurationTimerHandle, DA_DurationTime))
				{
					AttackCoolDownTimer();
					VelocityDecelerateTimer();
					GetWorldTimerManager().ClearTimer(DA_DurationTimerHandle);
					FPPTimerHelper::InvalidateTimerHandle(DA_DurationTimerHandle);
				}
			}), 0.01f, true);
		RollingMesh->SetPhysicsLinearVelocity(AD_Direction * DA_AddVelocityValue);
	}
}

void AKWPlayerCharacter::AttackCoolDownTimer()
{
	GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(AttackCoolDownTimerHandle, AttackCoolDownTime))
			{
				GetWorldTimerManager().ClearTimer(AttackCoolDownTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(AttackCoolDownTimerHandle);
			}
		}), 0.01f, true);
}

void AKWPlayerCharacter::VelocityDecelerateTimer()
{
	if(GetWorldTimerManager().IsTimerActive(VelocityDecelerationTimerHandle))
	{
		return;
	}
	if(CurrentGearState == EGearState::GearThree)
	{
		CurrentGearState = EGearState::GearTwo;
	}
	
	GetWorldTimerManager().SetTimer(VelocityDecelerationTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if(FPPTimerHelper::IsDelayElapsed(VelocityDecelerationTimerHandle, 0.005f))
			{
				if(bIsAttackOnGoing)
				{
					RollingMesh->AddForce(RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -50000);
				}
				else
				{
					RollingMesh->AddForce(RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -10000);
				}
			}
		
			if(abs(RollingMesh->GetPhysicsLinearVelocity().X) <= VelocityDecelerateTarget.X && abs(RollingMesh->GetPhysicsLinearVelocity().Y) <= VelocityDecelerateTarget.Y)
			{
				if(bIsAttackOnGoing)
				{
					bIsAttackOnGoing = false;
				}
				VelocityDecelerateTarget = FVector::ZeroVector;
				GetWorldTimerManager().ClearTimer(VelocityDecelerationTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(VelocityDecelerationTimerHandle);
			}
		
			if(!bIsRolling || (!bIsAttackOnGoing && bIsMoving) || abs(RollingMesh->GetPhysicsLinearVelocity().Z) > 20.f)
			{
				if(bIsAttackOnGoing)
				{
					bIsAttackOnGoing = false;
				}
				VelocityDecelerateTarget = FVector::ZeroVector;
				GetWorldTimerManager().ClearTimer(VelocityDecelerationTimerHandle);
				FPPTimerHelper::InvalidateTimerHandle(VelocityDecelerationTimerHandle);
			}
		}), 0.001f, true);
}

void AKWPlayerCharacter::CheckGearState()
{
	float LowerGearValue = 0.f;
	if(static_cast<uint8>(CurrentGearState) - 1 >= 0)
	{
		LowerGearValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState) - 1];
	}
						
	float UpperGearValue = 0.f;
	if(static_cast<uint8>(CurrentGearState) + 1 < static_cast<uint8>(EGearState::EndOfGearState))
	{
		UpperGearValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)];
	}
	
	if(LowerGearValue != 0.f && (abs(RollingMesh->GetPhysicsLinearVelocity().Size2D() / 2) < LowerGearValue || abs(RollingMesh->GetPhysicsLinearVelocity().X) < LowerGearValue || abs(RollingMesh->GetPhysicsLinearVelocity().Y) < LowerGearValue))
	{
		switch (CurrentGearState)
		{
		case EGearState::GearTwo:
			CurrentGearState = EGearState::GearOne;
			break;
		case EGearState::GearThree:
			CurrentGearState = EGearState::GearTwo;
			break;
		case EGearState::GearFour:
			CurrentGearState = EGearState::GearThree;
			break;
		default:
			checkNoEntry();
		}
		RollingMesh->SetVectorParameterValueOnMaterials("GlowColor", ColorsByGear[static_cast<uint8>(CurrentGearState)]);
	}

	// 왜 직선 기어값 변경은 두번 체크해야 하는지 이유 불명
	{	
		if(abs(RollingMesh->GetPhysicsLinearVelocity().X) > UpperGearValue || abs(RollingMesh->GetPhysicsLinearVelocity().Y) > UpperGearValue)
		{
			switch (CurrentGearState)
			{
			case EGearState::GearOne:
				CurrentGearState = EGearState::GearTwo;
				break;
			case EGearState::GearTwo:
				CurrentGearState = EGearState::GearThree;
				break;
			case EGearState::GearThree:
				break;
			default:
				checkNoEntry();
			}
		}
		if(abs(RollingMesh->GetPhysicsLinearVelocity().X) > UpperGearValue || abs(RollingMesh->GetPhysicsLinearVelocity().Y) > UpperGearValue)
		{
			switch (CurrentGearState)
			{
			case EGearState::GearOne:
				CurrentGearState = EGearState::GearTwo;
				break;
			case EGearState::GearTwo:
				CurrentGearState = EGearState::GearThree;
				break;
			case EGearState::GearThree:
				break;
			default:
				checkNoEntry();
			}
			RollingMesh->SetVectorParameterValueOnMaterials("GlowColor", ColorsByGear[static_cast<uint8>(CurrentGearState)]);
		}
	}
}


