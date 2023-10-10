// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Player/KWPlayerDataAsset.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KWPlayerAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"

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
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	GetCapsuleComponent()->SetCapsuleSize(70.f, 70.f);
	GetMesh()->BodyInstance.bLockZRotation = true;
	
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

	WalkingMesh = CharacterData-> PlayerWalkingMesh;
	PlayerWalkingAnimBlueprint = CharacterData->PlayerWalkingAnimBlueprint;

	GetMesh()->SetCollisionObjectType(ECC_PLAYER);
	GetMesh()->SetCollisionProfileName(CP_PLAYER, true);
	GetMesh()->SetSkeletalMesh(WalkingMesh);
	GetMesh()->SetAnimInstanceClass(FPPConstructorHelper::FindAndGetClass<UKWPlayerAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/11-Player-Anim/Animations/ABP_Player_Kiwi.ABP_Player_Kiwi_C'"), EAssertionLevel::Check));
	
	InputMappingContext= CharacterData->PlayerInputMappingContext;
	ToggleTypeAction = CharacterData->ToggleTypeAction;
	MoveInputAction = CharacterData->MoveInputAction;
	JumpAction = CharacterData->JumpAction;
	AttackAction = CharacterData->AttackAction;
	FileDriverAction = CharacterData->FileDriverAction;
	
	bCanDashOnFlying = CharacterData->bCanDashOnFlying;
	DefaultVelocityValue = CharacterData->DefaultVelocityValue;
	DefaultMaxVelocityValue = CharacterData->DefaultMaxVelocityValue;
	SystemMaxVelocityValue = CharacterData->SystemMaxVelocityValue;
	CurrentMaxVelocityValue = DefaultMaxVelocityValue;
	MaxVelocityMagnificationByGear = CharacterData->MaxVelocityMagnificationByGear;

	for(int ArrayNum = 0; ArrayNum < static_cast<uint8>(EGearState::GearTwo); ArrayNum++)
	{
		MaxVelocityByGear.Emplace(DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[ArrayNum]);
	}
	
	RB_MultiplyValuesByGear = CharacterData->RB_MultiplyValuesByGear;
	RB_MultiplyValuesByObjectType = CharacterData->RB_MultiplyValuesByObjectType;
	RBD_JustTimingValue = CharacterData->RBD_JustTimingValue;
	RB_DisableMovementTime = CharacterData->RB_DisableMovementTime;
	RBD_AddVelocityValue = CharacterData->RBD_AddVelocityValue;
	
	AddJumpForceValue = CharacterData->AddJumpForceValue;
	JumpDelayTime = CharacterData->JumpDelayTime;
	
	DA_AddVelocityValue = CharacterData->DA_AddVelocityValue;
	DA_DurationTime = CharacterData->DA_DurationTime;
	DA_DecelerateValue = CharacterData->DA_DecelerateValue;
	AttackCoolDownTime = CharacterData->AttackCoolDownTime;

	DropDownVelocityValue = CharacterData->DropDownVelocityValue;
	DropDownMinimumHeightValue = CharacterData->DropDownMinimumHeightValue;
	DropDownCoolDownTime = CharacterData->DropDownCoolDownTime;
	
	ColorsByGear = CharacterData->ColorsByGear;
}

// Called when the game starts or when spawned
void AKWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	RollingMesh->SetCollisionObjectType(ECC_PLAYER);
	RollingMesh->SetCollisionProfileName(CP_PLAYER, true);
	RollingMesh->SetMassOverrideInKg(NAME_None, 50.f);
	RollingMesh->SetStaticMesh(nullptr);
	RollingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RollingMesh->BodyInstance.bLockZRotation = true;

	Camera->FieldOfView = CharacterData->CameraFOV;
	SpringArm->SetRelativeLocation(PlayerComponent->GetRelativeLocation());
	SpringArm->TargetArmLength =CharacterData->SpringArmLength;
	SpringArm->SetRelativeRotation(FRotator(CharacterData->SpringArmAngle, 0.f,0.f));
	PlayerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->MaxWalkSpeed = CharacterData->WakingStateMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = CharacterData->WakingStateJumpValue;
	GetCharacterMovement()->GravityScale = CharacterData->WakingStateGravityScale;
	
	CurrentGearState = EGearState::GearOne;
	bIsDead = false;
	bIsMoving = false;
	bIsRolling = false;
	bIsFlying = false;
	bIsUsedFlyDash = false;
	bIsReBounding = false;
	bIsInputJustAction = false;
	bIsAttackOnGoing = false;
	bIsCanInputJustAction = true;
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
	
	if(!bIsMoving && bIsRolling && RollingMesh->GetPhysicsLinearVelocity().Length() > 100.f)
	{
		VelocityDecelerateTimer();
	}
	
	if(bIsRolling)
	{
		CheckGearState();
		const FLinearColor Color = ColorsByGear[static_cast<uint8>(CurrentGearState)];
		const FVector ColorVector = FVector(Color.R, Color.G, Color.B);
		RollingMesh->SetVectorParameterValueOnMaterials("GlowColor", ColorVector);
		
		FVector PlaneVelocityVector = RollingMesh->GetPhysicsLinearVelocity();
		if(abs(PlaneVelocityVector.Z) > DropDownMinimumHeightValue)
		{
			bIsFlying = true;
		}

		// 체공 체크용 충돌 체크.
		// 추후 공 모델링 적용시 소켓을 사용하여 바닥 위치 지정
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() - FVector(0.f, 0.f, 70.f),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(FVector(10.f, 10.f, 10.f)),
		Params);
			
		if(bResult)
		{
			bIsFlying = false;
			bIsUsedFlyDash = false;
		}
		
		PlaneVelocityVector.Z = 0.f;
		if(float VelocityLength = PlaneVelocityVector.Length() > SystemMaxVelocityValue * 2 && !bIsAttackOnGoing)
		{
			float LengthX = FMath::Clamp(PlaneVelocityVector.X, -SystemMaxVelocityValue, SystemMaxVelocityValue);
			float LengthY = FMath::Clamp(PlaneVelocityVector.Y, -SystemMaxVelocityValue, SystemMaxVelocityValue);
			float LengthZ = RollingMesh->GetPhysicsLinearVelocity().Z;
			
			if (bIsReBounding)
			{
				float ReBoundingClampValue = SystemMaxVelocityValue;
				LengthX = FMath::Clamp(PlaneVelocityVector.X, -ReBoundingClampValue, ReBoundingClampValue);
				LengthY = FMath::Clamp(PlaneVelocityVector.Y, -ReBoundingClampValue, ReBoundingClampValue);
				LengthZ = FMath::Clamp(LengthZ, -ReBoundingClampValue, ReBoundingClampValue);
			}
			
			if (LengthZ > SystemMaxVelocityValue)
			{
				LengthZ = FMath::Clamp(LengthZ, -SystemMaxVelocityValue, SystemMaxVelocityValue);
			}
			RollingMesh->SetPhysicsLinearVelocity(FVector(LengthX, LengthY, LengthZ));
		}
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

	EnhancedInputComponent->BindAction(FileDriverAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::DropDownActionSequence);
}

void AKWPlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	if(bIsReBounding || bIsAttackOnGoing)
	{
		return;
	}
	bIsMoving = true;
	MoveInputValue = Value.Get<FVector2D>();
	float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}

	// Camera->GetForwardVector().X;
	// Camera->GetRightVector().Y;

	FVector MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.f);
	
	if(bIsRolling && CurrentGearState != EGearState::GearThree && CurrentGearState != EGearState::GearFour)
	{
		FVector AddVelocityResult = MoveDirection * DefaultVelocityValue;
		AddVelocityResult.Z = 0.f;
		
		if(RollingMesh->GetPhysicsLinearVelocity().X >= CurrentMaxVelocityValue && MoveDirection.X > 0)
		{
			AddVelocityResult.X = 0.f;
		}
		else if(RollingMesh->GetPhysicsLinearVelocity().X <= -CurrentMaxVelocityValue && MoveDirection.X < 0)
		{
			AddVelocityResult.X = 0.f;
		}
		
		if(RollingMesh->GetPhysicsLinearVelocity().Y >= CurrentMaxVelocityValue && MoveDirection.Y > 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		else if(RollingMesh->GetPhysicsLinearVelocity().Y <= -CurrentMaxVelocityValue && MoveDirection.Y < 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		
		if(MoveInputValue.X == 0 && RollingMesh->GetPhysicsLinearVelocity().X > 10.f)
		{
			AddVelocityResult.X = -10.f;
		}
		else if(MoveInputValue.X == 0 && RollingMesh->GetPhysicsLinearVelocity().X < -10.f)
		{
			AddVelocityResult.X = 10.f;
		}
		
		if(MoveInputValue.Y == 0 && RollingMesh->GetPhysicsLinearVelocity().Y > 10.f)
		{
			AddVelocityResult.Y = -10.f;
		}
		else if(MoveInputValue.Y == 0 && RollingMesh->GetPhysicsLinearVelocity().Y < -10.f)
		{
			AddVelocityResult.Y = 10.f;
		}
		
		RollingMesh->SetPhysicsLinearVelocity(RollingMesh->GetPhysicsLinearVelocity() + AddVelocityResult);
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
		CurrentGearState = EGearState::GearOne;
		VelocityDecelerateTimer();
	}
}

void AKWPlayerCharacter::JumpAddForceAction(const FInputActionValue& Value)
{
	
	if(bIsFlying || bIsJumpCoolDown)
	{
		return;
	}
	bIsJumpCoolDown = true;
	GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, this, &AKWPlayerCharacter::JumpCoolDownEnd, JumpDelayTime, false);
	
	bIsFlying = true;
	if(bIsRolling)
	{
		const FVector JumpVelocityVector = FVector(0.f, 0.f, AddJumpForceValue);
		RollingMesh->SetPhysicsLinearVelocity(RollingMesh->GetPhysicsLinearVelocity() + JumpVelocityVector);
	}
	else
	{
		Jump();
	}
}

void AKWPlayerCharacter::JumpCoolDownEnd()
{
	bIsJumpCoolDown = false;
	bIsFlying = false;
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

void AKWPlayerCharacter::AttackActionSequence(const FInputActionValue& Value)
{
	if(!bIsRolling)
	{
		return;
	}
	if(bIsReBounding)
	{
		if(!bIsCanInputJustAction)
		{
			return;
		}
		bIsCanInputJustAction = false;
		RBD_JustTimingProceedAction();
	}
	else
	{
		if(bIsAttackCoolDown)
		{
			return;
		}

		if(bCanDashOnFlying && !bIsUsedFlyDash)
		{
			bIsUsedFlyDash = true;
			DA_ProceedAction();
			return;
		}
			
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
			DA_ProceedAction();
		}
	}
}

void AKWPlayerCharacter::DropDownActionSequence(const FInputActionValue& Value)
{
	if(bIsReBounding || bIsDropDownOnGoing || bIsDropDownCoolDown)
	{
		return;
	}

	if(bIsFlying && abs(RollingMesh->GetPhysicsLinearVelocity().Z) > DropDownMinimumHeightValue)
	{
		FD_ProceedAction();
	}
}

void AKWPlayerCharacter::AttackCoolDownTimer()
{
	bIsAttackCoolDown = false;
}

void AKWPlayerCharacter::DropDownCoolDownTimer()
{
	bIsDropDownCoolDown = false;
}

void AKWPlayerCharacter::VelocityDecelerateTimer()
{
	if(bIsDeceleration || bIsReBounding)
	{
		return;
	}
	bIsDeceleration = true;
	if(CurrentGearState == EGearState::GearThree)
	{
		CurrentGearState = EGearState::GearTwo;
	}

	GetWorldTimerManager().SetTimer(VelocityDecelerationTimerHandle, this, &AKWPlayerCharacter::VelocityDecelerationExecute, 0.001f, true);
}

void AKWPlayerCharacter::VelocityDecelerationExecute()
{
	if(bIsAttackOnGoing)
	{
		RollingMesh->AddForce(RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -10000 * (1 + DA_DecelerateValue));
	}
	else
	{
		RollingMesh->AddForce(RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -10000);
	}
		
	if(abs(RollingMesh->GetPhysicsLinearVelocity().X) <= abs(VelocityDecelerateTarget.X) && abs(RollingMesh->GetPhysicsLinearVelocity().Y) <= abs(VelocityDecelerateTarget.Y))
	{
		if(bIsAttackOnGoing)
		{
			bIsAttackOnGoing = false;
		}
		VelocityDecelerateTarget = FVector::ZeroVector;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Decelerate End")));
		bIsDeceleration = false;
		GetWorldTimerManager().ClearTimer(VelocityDecelerationTimerHandle);
	}
		
	if(!bIsRolling || (!bIsAttackOnGoing && bIsMoving) || abs(RollingMesh->GetPhysicsLinearVelocity().Z) > 20.f)
	{
		if(bIsAttackOnGoing)
		{
			bIsAttackOnGoing = false;
		}
		VelocityDecelerateTarget = FVector::ZeroVector;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Decelerate End")));
		bIsDeceleration = false;
		GetWorldTimerManager().ClearTimer(VelocityDecelerationTimerHandle);
	}
}

void AKWPlayerCharacter::RBD_JustTimingProceedAction()
{
	bIsInputJustAction = true;
	GetWorldTimerManager().SetTimer(RBD_ReBoundCheckHandle,this, &AKWPlayerCharacter::RBD_Check, 0.01f, true);
	GetWorldTimerManager().SetTimer(RBD_JustTimingCheckHandle,this, &AKWPlayerCharacter::RBD_TimeOut, RBD_JustTimingValue, false);
}

void AKWPlayerCharacter::RBD_Check()
{
	if(!bIsReBounding)
	{
		bIsCanInputJustAction = false;
		if(GetWorldTimerManager().IsTimerActive(RBD_ReBoundCheckHandle))
		{
			GetWorldTimerManager().ClearTimer(RBD_ReBoundCheckHandle);
		}
	}
}

void AKWPlayerCharacter::RBD_TimeOut()
{
	bIsInputJustAction = false;
}

void AKWPlayerCharacter::DA_ProceedAction()
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
	FVector AD_Direction = (MousePosition3D - ScreenCenter3D).GetSafeNormal() * DA_AddVelocityValue;
	AD_Direction.Z = RollingMesh->GetPhysicsLinearVelocity().Z;
		
	VelocityDecelerateTarget = RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * CurrentMaxVelocityValue;
	VelocityDecelerateTarget.Z = 0.f;
	if(VelocityDecelerateTarget.Length() < 100.f)
	{
		VelocityDecelerateTarget = FVector(100.f, 100.f, 0.f);
	}
	GetWorldTimerManager().SetTimer(DA_DurationTimerHandle, this, &AKWPlayerCharacter::DA_EndAction, DA_DurationTime, false);
	RollingMesh->SetPhysicsLinearVelocity(AD_Direction);
}

void AKWPlayerCharacter::DA_EndAction()
{
	CurrentGearState = EGearState::GearTwo;
	bIsAttackCoolDown = true;
	GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, this, &AKWPlayerCharacter::AttackCoolDownTimer, AttackCoolDownTime, false);
	VelocityDecelerateTimer();
}

void AKWPlayerCharacter::FD_ProceedAction()
{
	RollingMesh->SetSimulatePhysics(false);
	bIsDropDownOnGoing = true;
	GetWorldTimerManager().SetTimer(DropDownTimerHandle, this, &AKWPlayerCharacter::FD_Execute, 0.1f, false);
}

void AKWPlayerCharacter::FD_Execute()
{
	RollingMesh->SetSimulatePhysics(true);
	const FVector DroppingVelocity = FVector(0.f, 0.f, -DropDownVelocityValue);
	RollingMesh->SetPhysicsLinearVelocity(DroppingVelocity);
	GetWorldTimerManager().SetTimer(DropDownCoolDownTimerHandle, this, &AKWPlayerCharacter::DropDownCoolDownTimer, DropDownCoolDownTime, false);
	bIsDropDownOnGoing = false;
}

void AKWPlayerCharacter::CheckGearState()
{
	if(bIsAttackOnGoing || GetWorldTimerManager().IsTimerActive(CheckGearStateTimerHandle))
	{
		return;
	}
	GetWorldTimerManager().SetTimer(CheckGearStateTimerHandle, this, &AKWPlayerCharacter::CheckGearStateExecute, 1.0f, true);
}

void AKWPlayerCharacter::CheckGearStateExecute()
{
	if(!bIsRolling)
	{
		GetWorldTimerManager().ClearTimer(CheckGearStateTimerHandle);
		FPPTimerHelper::InvalidateTimerHandle(CheckGearStateTimerHandle);
	}
	
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
		const FLinearColor Color = ColorsByGear[static_cast<uint8>(CurrentGearState)];
		const FVector ColorVector = FVector(Color.R, Color.G, Color.B);
		RollingMesh->SetVectorParameterValueOnMaterials("GlowColor", ColorVector);
		CurrentMaxVelocityValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)];
		return;
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
				// CurrentGearState = EGearState::GearThree;
				break;
			case EGearState::GearThree:
				break;
			case EGearState::GearFour:
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
				// CurrentGearState = EGearState::GearThree;
				break;
			case EGearState::GearThree:
				break;
			case EGearState::GearFour:
				break;
			default:
				checkNoEntry();
			}
			CurrentMaxVelocityValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)];
		}
	}
}

void AKWPlayerCharacter::RB_ApplyReBoundByObjectType(FVector& ReBoundResultValue, EReBoundObjectType ObjectType)
{
	if(!bIsRolling || bIsReBounding || GetWorldTimerManager().IsTimerActive(RB_DelayTimerHandle))
	{
		return;
	}
	bIsReBounding = true;
	RollingMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);

	ReBoundResultValue *= RB_MultiplyValuesByObjectType[static_cast<uint8>(ObjectType)];
	ReBoundResultValue *= RB_MultiplyValuesByGear[static_cast<uint8>(CurrentGearState)];
	RollingMesh->SetPhysicsLinearVelocity(ReBoundResultValue);
	GetWorldTimerManager().SetTimer(RB_ContactCheckHandle,this , &AKWPlayerCharacter::RB_CheckContactToFloor, 0.01f, true);
}

void AKWPlayerCharacter::RB_CheckContactToFloor()
{
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
		bIsReBounding = false;
		if(bIsInputJustAction)
		{
			bIsReBounding = false;
			bIsInputJustAction = false;
			RBD_SuccessEvent();
		}
		else
		{
			RBD_FailedEvent();
		}
		bIsCanInputJustAction = true;
		GetWorldTimerManager().ClearTimer(RB_ContactCheckHandle);
	}
}

void AKWPlayerCharacter::RBD_SuccessEvent()
{
	// 리바운드 대쉬 실행
	bIsAttackOnGoing = true;
	CurrentGearState = EGearState::GearFour;
	FVector2D MousePosition;
	int ScreenSizeX;
	int ScreenSizeY;
		
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);
		
	const FVector MousePosition3D = FVector(-MousePosition.Y, MousePosition.X, 0.f);
	const FVector ScreenCenter3D = FVector(-ScreenSizeY / 2, ScreenSizeX / 2, 0.0f);
	const FVector AD_Direction = (MousePosition3D - ScreenCenter3D).GetSafeNormal();

	VelocityDecelerateTarget = RollingMesh->GetPhysicsLinearVelocity().GetSafeNormal() * CurrentMaxVelocityValue * 0.75;
	
	GetWorldTimerManager().SetTimer(RBD_SucceedTimerHandle, this,&AKWPlayerCharacter::RBD_SuccessEventExecute,DA_DurationTime, false);
	RollingMesh->SetPhysicsLinearVelocity(AD_Direction * RBD_AddVelocityValue);
}

void AKWPlayerCharacter::RBD_SuccessEventExecute()
{
	CurrentGearState = EGearState::GearTwo;
	VelocityDecelerateTimer();
}

void AKWPlayerCharacter::RBD_FailedEvent()
{
	// 입력 실패로 간주하고 실패 애니메이션 적용 및 입력 비활성화
	DisableInput(Cast<APlayerController>(GetController()));
	GetWorldTimerManager().SetTimer(RBD_FailedTimerHandle,this , &AKWPlayerCharacter::RBD_FailedEventExecute, RB_DisableMovementTime, false);
}

void AKWPlayerCharacter::RBD_FailedEventExecute()
{
	EnableInput(Cast<APlayerController>(GetController()));
}


