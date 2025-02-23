// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
#include "XR_Project_Team10/Player/KWPlayerDataAsset.h"
#include "XR_Project_Team10/Object/KWLocationDetector.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"
#include "XR_Project_Team10/Util/PPTimerHelper.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KWPlayerAnimInstance.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "XR_Project_Team10/Character/Monster/Boss/KWBossMonsterHohonu.h"
#include "XR_Project_Team10/CommonMonster/CommonMonster.h"
#include "XR_Project_Team10/CommonMonster/RuneSpirit.h"
#include "XR_Project_Team10/Constant/KWAnimMontageSectionName.h"
#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Game/KWGameInstance.h"

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

	MeshOriginScale = FVector::OneVector * 2.5f;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(MeshOriginScale);
	GetCapsuleComponent()->SetCapsuleSize(70.f, 70.f);
	GetMesh()->BodyInstance.bLockZRotation = true;
	
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootMesh->SetSimulatePhysics(true);

	RollingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RollingMesh"));
	RollingMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	RollingMeshComponent->SetRelativeScale3D(MeshOriginScale);
	PlayerComponent = Cast<UCapsuleComponent>(RootComponent);
	RootComponent = RootMesh;
	PlayerComponent->SetupAttachment(RootComponent);
	RollingMeshComponent->SetupAttachment(RootComponent);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetUsingAbsoluteRotation(false);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	KiwiModeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("KiwiNiagaraComponent"));
	KiwiModeNiagaraComponent->SetupAttachment(GetMesh());
	RollingModeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RollingNiagaraComponent"));
	AttackEffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AttackNiagaraComponent"));
	AttackEffectNiagaraComponent->SetupAttachment(RollingMeshComponent);
	EventNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EventNiagaraComponent"));
	EventNiagaraComponent->SetRelativeScale3D(FVector::One() * 2);

	KiwiMainAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MainAudioComponent"));
	KiwiMainAudioComponent->SetupAttachment(RootComponent);
	KiwiSubAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SubAudioComponent"));
	KiwiSubAudioComponent->SetupAttachment(RootComponent);
	
	PlayerWidgetController = CreateDefaultSubobject<UKWPlayerWidgetController>(TEXT("PlayerWidgetController"));

	CharacterData = FPPConstructorHelper::FindAndGetObject<UKWPlayerDataAsset>(TEXT("/Script/XR_Project_Team10.KWPlayerDataAsset'/Game/Rolling-Kiwi/Datas/DataAssets/PlayerDataAsset.PlayerDataAsset'"), EAssertionLevel::Check);
	RootStaticMesh = FPPConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/1-Graphic-Resource/Player/SM_DummyBall.SM_DummyBall'"), EAssertionLevel::Check);

	PlayerHp = CharacterData->PlayerHp;
	
	WalkingMesh = CharacterData->PlayerWalkingMesh;
	RollingMesh = CharacterData->PlayerRollingMesh;
	
	//TODO: 폴더 정리 후 ConstructorHelper로 애님 인스턴스 가져오기
	WalkingAnimInstance = FPPConstructorHelper::FindAndGetClass<UKWPlayerAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/1-Graphic-Resource/Player/Animation/ABP_Player_Kiwi.ABP_Player_Kiwi_C'"), EAssertionLevel::Check);
	
	RollingAnimInstance = FPPConstructorHelper::FindAndGetClass<UAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/1-Graphic-Resource/Player/Animation/ABP_Player_Kiwi_Roll.ABP_Player_Kiwi_Roll_C'"), EAssertionLevel::Check);
	
	KiwiAnimMontage = CharacterData->KiwiAnimMontage;
	
	InputMappingContext= CharacterData->PlayerInputMappingContext;
	ToggleTypeAction = CharacterData->ToggleTypeAction;
	MoveInputAction = CharacterData->MoveInputAction;
	JumpAction = CharacterData->JumpAction;	
	AttackAction = CharacterData->AttackAction;
	FileDriverAction = CharacterData->DropDownAction;
	PauseGameAction = CharacterData->PauseGameAction;

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
	RBD_JustTimingCheckTime = CharacterData->RBD_JustTimingCheckTime;
	RB_DisableMovementTime = CharacterData->RB_DisableMovementTime;
	RBD_AddVelocityValue = CharacterData->RBD_AddVelocityValue;
	
	AddJumpForceValue = CharacterData->AddJumpForceValue;
	JumpDelayTime = CharacterData->JumpDelayTime;

	RollingModeNiagaraComponent->SetAsset(CharacterData->DA_MoveNiagaraSystem);
	DA_BaseDamage = CharacterData->DA_BaseDamage;
	DA_MultiplyDamageByGear = CharacterData->DA_MultiplyDamageByGear;
	DA_AddVelocityValue = CharacterData->DA_AddVelocityValue;
	DA_DurationTime = CharacterData->DA_DurationTime;
	DA_DecelerateValue = CharacterData->DA_DecelerateValue;
	AttackCoolDownTime = CharacterData->AttackCoolDownTime;

	DropDownDamage = CharacterData->DropDownDamage;
	DropDownVelocityValue = CharacterData->DropDownVelocityValue;
	DropDownMinimumHeightValue = CharacterData->DropDownMinimumHeightValue;
	DropDownCoolDownTime = CharacterData->DropDownCoolDownTime;
	
	ColorsByGear = CharacterData->ColorsByGear;

	bIsEnableHitCheckDebugView = false;
	bIsEnableLocationDebugView = false;
	bIsEnableGearDebugView = false;
	bIsEnableVelocityDebugView = false;

}

// Called when the game starts or when spawned
void AKWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerTrueLocation = GetWorld()->SpawnActor<AKWLocationDetector>();
	PlayerTrueLocation->SetTargetCharacter(this);
	PlayerTrueLocation->SetHitChannelType(ECC_PLAYER);
	PlayerTrueLocation->SetHitChannelProfile(CP_PLAYER);
	PlayerTrueLocation->SetHitBoxExtent(70.f);

	GetCapsuleComponent()->SetCollisionObjectType(ECC_PLAYER);
	GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER, true);
	
	GetMesh()->SetCollisionObjectType(ECC_PLAYER);
	GetMesh()->SetCollisionProfileName(CP_PLAYER, true);
	GetMesh()->SetSkeletalMesh(WalkingMesh);
	GetMesh()->SetAnimInstanceClass(WalkingAnimInstance);
	UKWPlayerAnimInstance* PlayerAnimInstance = Cast<UKWPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if(PlayerAnimInstance)
	{
		PlayerAnimInstance->EndDeadAnimDelegate.AddUObject(this, &AKWPlayerCharacter::StartFadeOut);
	}
	RollingMeshComponent->SetCollisionObjectType(ECC_PLAYER);
	RollingMeshComponent->SetCollisionProfileName(CP_PLAYER, true);
	RollingMeshComponent->SetSkeletalMesh(RollingMesh);
	RollingMeshComponent->SetAnimInstanceClass(RollingAnimInstance);
	RollingMeshComponent->SetWorldScale3D(FVector::ZeroVector);
	
	RootMesh->SetCollisionObjectType(ECC_PLAYER);
	RootMesh->SetCollisionProfileName(CP_PLAYER, true);
	RootMesh->SetMassOverrideInKg(NAME_None, 50.f);
	RootMesh->SetStaticMesh(RootStaticMesh);
	RootMesh->SetScalarParameterValueOnMaterials("Opacity", 0.f);
	RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootMesh->BodyInstance.bLockZRotation = true;
	
	Camera->FieldOfView = CharacterData->CameraFOV;
	SpringArm->SetRelativeLocation(PlayerComponent->GetRelativeLocation());
	SpringArm->TargetArmLength =CharacterData->SpringArmLength;
	SpringArm->SetRelativeRotation(FRotator(CharacterData->SpringArmAngle, 0.f,0.f));
	SpringArm->AttachToComponent(PlayerTrueLocation->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	bIsEnableHitCheckDebugView = CharacterData->bIsEnableHitCheckDebugView;
	bIsEnableGearDebugView = CharacterData->bIsEnableGearDebugView;
	bIsEnableVelocityDebugView = CharacterData->bIsEnableVelocityDebugView;
	bIsEnableLocationDebugView = CharacterData->bIsEnableLocationDebugView;
	bIsMovingMustRolling = CharacterData->bIsMovingMustRolling;
	bIsRollingIdleToWalk = CharacterData->bIsRollingIdleToWalk;
	
	PlayerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->MaxWalkSpeed = CharacterData->WakingStateMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = CharacterData->WakingStateJumpValue;	
	GetCharacterMovement()->GravityScale = CharacterData->WakingStateGravityScale;
	GetCharacterMovement()->AirControl = CharacterData->WakingStateAirControl;
	
	CurrentGearState = EGearState::GearOne;
	bIsDead = false;
	bIsMoving = false;
	bIsRolling = false;
	bIsFlying = false;
	bIsUsedFlyDash = false;
	bIsReBounding = false;
	bIsInputJustAction = false;
	bIsAttackOnGoing = false;
	bIsKnockBackOnGoing = false;

	RollingModeNiagaraComponent->AttachToComponent(PlayerTrueLocation->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	EventNiagaraComponent->AttachToComponent(PlayerTrueLocation->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	KiwiModeNiagaraComponent->Deactivate();
	RollingModeNiagaraComponent->Deactivate();
	EventNiagaraComponent->Deactivate();
}

float AKWPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if(PlayerHp <= 0 || GetWorldTimerManager().IsTimerActive(DamageDelayTimerHandle))
	{
		return 0;
	}

	if(!GetWorldTimerManager().IsTimerActive(DamageDelayTimerHandle))
	{
		GetWorldTimerManager().SetTimer(DamageDelayTimerHandle, this, &AKWPlayerCharacter::DamageDelayTimer, 2.f, false);
	}
	
	if(bIsRolling)
	{
		EventNiagaraComponent->SetRelativeLocation(RollingMeshComponent->GetRelativeLocation());
	}
	else
	{
		EventNiagaraComponent->SetWorldLocation(PlayerTrueLocation->GetActorLocation());
	}
	
	EventNiagaraComponent->SetAsset(CharacterData->HitNiagaraSystem);
	EventNiagaraComponent->Activate(true);
	GetMesh()->GetAnimInstance()->Montage_Play(KiwiAnimMontage);

	PlayerHp -= DamageAmount;
	PlayerWidgetController->UpdateHealthWidget(false);
	if(PlayerHp <= 0)
	{	
		if(bIsRolling)
		{
			ToggleCharacterType();
		}
		DisableInput(GetWorld()->GetFirstPlayerController());
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_DEAD, KiwiAnimMontage);
		return 0;
	}
	// 피격 애니메이션 실행
	return 0;
}

// Called every frame
void AKWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!bIsRolling)
	{
		PlayerTrueLocation->SetActorLocation(PlayerComponent->GetComponentToWorld().GetLocation());
	}
	else
	{	
		RollingMeshComponent->SetWorldLocation(GetActorLocation() + FVector(0.f, 0.f, -0.f));
		PlayerTrueLocation->SetActorLocation(GetActorLocation());
	}

	PlayerWidgetController->UpdateGearWidget(CurrentGearState);
	
	if(!bIsRolling)
	{
		return;
	}

	if(!bIsMoving && bIsRolling && !bIsAttackOnGoing && !bIsAccelerated && !bIsDecelerateOnGoing && RootMesh->GetPhysicsLinearVelocity().Length() > 100.f)
	{
		BeginDeceleration();
	}
	
	if(!GetWorldTimerManager().IsTimerActive(CheckGearStateTimerHandle))
	{
		GetWorldTimerManager().SetTimer(CheckGearStateTimerHandle, this, &AKWPlayerCharacter::CheckGearState, 3.f, true);
	}
	// 체공 체크용 충돌 체크.
	// 추후 공 모델링 적용시 소켓을 사용하여 바닥 위치 지정
	
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, DropDownMinimumHeightValue);
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->LineTraceSingleByProfile(
		HitResult,
		StartLocation,
		EndLocation,
		CP_STATIC_ONLY,
		Params
	);
			
	if(bResult)
	{
		bIsFlying = false;
		bIsUsedFlyDash = false;
	}
	else
	{
		bIsFlying = true;
	}
	
	FVector PlaneVelocityVector = RootMesh->GetPhysicsLinearVelocity();
	PlaneVelocityVector.Z = 0.f;
	if(PlaneVelocityVector.Length() > SystemMaxVelocityValue * 2 && !bIsAttackOnGoing && !bIsAccelerated)
	{
		float LengthX = FMath::Clamp(PlaneVelocityVector.X, -SystemMaxVelocityValue, SystemMaxVelocityValue);
		float LengthY = FMath::Clamp(PlaneVelocityVector.Y, -SystemMaxVelocityValue, SystemMaxVelocityValue);
		float LengthZ = RootMesh->GetPhysicsLinearVelocity().Z;
			
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
		RootMesh->SetPhysicsLinearVelocity(FVector(LengthX, LengthY, LengthZ));
	}

	if(bIsEnableGearDebugView)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%d"), static_cast<uint8>(CurrentGearState)));
	}

	if(bIsEnableLocationDebugView)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f %f %f"),
		PlayerTrueLocation->GetActorLocation().X,
		PlayerTrueLocation->GetActorLocation().Y,
		PlayerTrueLocation->GetActorLocation().Z));
	}

		if(bIsEnableVelocityDebugView)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f %f %f"),
		RootMesh->GetPhysicsLinearVelocity().X,
		RootMesh->GetPhysicsLinearVelocity().Y,
		RootMesh->GetPhysicsLinearVelocity().Z));
	}
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

	EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this, &AKWPlayerCharacter::TogglePauseWidget);
}

void AKWPlayerCharacter::MoveAction(const FInputActionValue& Value)
{
	if(bIsReBounding || bIsAttackOnGoing || bIsKnockBackOnGoing)
	{
		return;
	}
	
	if(!bIsRolling && bIsMovingMustRolling)
	{
		ToggleCharacterType();
	}
	
	bIsMoving = true;
	MoveInputValue = Value.Get<FVector2D>();
	float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}

	FVector MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.f);
	
	if(bIsRolling && CurrentGearState != EGearState::GearThree && CurrentGearState != EGearState::GearFour)
	{
		FVector AddVelocityResult = MoveDirection * DefaultVelocityValue;
		AddVelocityResult.Z = 0.f;
		
		if(RootMesh->GetPhysicsLinearVelocity().X >= CurrentMaxVelocityValue && MoveDirection.X > 0)
		{
			AddVelocityResult.X = 0.f;
		}
		else if(RootMesh->GetPhysicsLinearVelocity().X <= -CurrentMaxVelocityValue && MoveDirection.X < 0)
		{
			AddVelocityResult.X = 0.f;
		}
		
		if(RootMesh->GetPhysicsLinearVelocity().Y >= CurrentMaxVelocityValue && MoveDirection.Y > 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		else if(RootMesh->GetPhysicsLinearVelocity().Y <= -CurrentMaxVelocityValue && MoveDirection.Y < 0)
		{
			AddVelocityResult.Y = 0.f;
		}
		
		if(MoveInputValue.X == 0 && RootMesh->GetPhysicsLinearVelocity().X > 10.f)
		{
			AddVelocityResult.X = -10.f;
		}
		else if(MoveInputValue.X == 0 && RootMesh->GetPhysicsLinearVelocity().X < -10.f)
		{
			AddVelocityResult.X = 10.f;
		}
		
		if(MoveInputValue.Y == 0 && RootMesh->GetPhysicsLinearVelocity().Y > 10.f)
		{
			AddVelocityResult.Y = -10.f;
		}
		else if(MoveInputValue.Y == 0 && RootMesh->GetPhysicsLinearVelocity().Y < -10.f)
		{
			AddVelocityResult.Y = 10.f;
		}

		if(!RollingModeNiagaraComponent->IsActive())
		{
			RollingModeNiagaraComponent->SetAsset(CharacterData->RollingNiagaraSystem);
			RollingModeNiagaraComponent->Activate();
		}
		RootMesh->SetPhysicsLinearVelocity(RootMesh->GetPhysicsLinearVelocity() + AddVelocityResult);
	}
	else
	{
		if(!KiwiModeNiagaraComponent->IsActive())
		{
			KiwiModeNiagaraComponent->SetAsset(CharacterData->WalkingNiagaraSystem);
			KiwiModeNiagaraComponent->Activate();
		}
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
		if(!bIsAttackOnGoing && !bIsFlying && !bIsDecelerateOnGoing)
		{
			BeginDeceleration();
		}
	}
	else
	{
		KiwiModeNiagaraComponent->Deactivate();
	}
}

void AKWPlayerCharacter::JumpAddForceAction(const FInputActionValue& Value)
{
	
	if(bIsFlying || GetWorldTimerManager().IsTimerActive(JumpDelayTimerHandle))
	{
		return;
	}
	
	GetWorldTimerManager().SetTimer(JumpDelayTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if(FPPTimerHelper::IsDelayElapsed(JumpDelayTimerHandle, JumpDelayTime))
		{
			bIsFlying = false;
			GetWorldTimerManager().ClearTimer(JumpDelayTimerHandle);
		}
	}), 0.01f, true);
	
	bIsFlying = true;
	if(bIsRolling)
	{
		const FVector JumpVelocityVector = FVector(0.f, 0.f, AddJumpForceValue);
		EventNiagaraComponent->SetAsset(CharacterData->JumpingNiagaraSystem);
		EventNiagaraComponent->Activate();
		RootMesh->SetPhysicsLinearVelocity(RootMesh->GetPhysicsLinearVelocity() + JumpVelocityVector);
	}
	else
	{
		Jump();
	}
}

void AKWPlayerCharacter::ToggleCharacterTypeAction(const FInputActionValue& Value)
{
	ToggleCharacterType();
}

void AKWPlayerCharacter::AttackActionSequence(const FInputActionValue& Value)
{
	if(!bIsRolling || bIsAttackOnGoing || (!bIsMoving && !bIsReBounding) || bIsKnockBackOnGoing)
	{
		return;
	}
	if(bIsReBounding)
	{
		if(GetWorldTimerManager().IsTimerActive(RBD_JustTimingCheckHandle))
		{
			return;
		}
		bIsInputJustAction = true;
		GetWorldTimerManager().SetTimer(RBD_JustTimingCheckHandle, this, &AKWPlayerCharacter::RBD_JustTimingProceedAction, 0.01f, false, RBD_JustTimingCheckTime);
	}
	else
	{
		if(GetWorldTimerManager().IsTimerActive(AttackCoolDownTimerHandle))
		{
			return;
		}

		AttackEffectNiagaraComponent->ResetSystem();
		AttackEffectNiagaraComponent->SetAsset(CharacterData->DA_MoveNiagaraSystem);
		AttackEffectNiagaraComponent->Activate();
		
		if(bCanDashOnFlying && !bIsUsedFlyDash && !DA_ElapsedTime)
		{
			bIsUsedFlyDash = true;
			DA_ProceedAction();
			return;
		}
			
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		
		bool bResult = GetWorld()->SweepSingleByProfile(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		CP_PLAYER,
		FCollisionShape::MakeSphere(70.0f),
		Params);
			
		if(bResult && !DA_ElapsedTime)
		{
			DA_ProceedAction();
		}
	}
}

void AKWPlayerCharacter::DropDownActionSequence(const FInputActionValue& Value)
{
	if(bIsReBounding || bIsKnockBackOnGoing || GetWorldTimerManager().IsTimerActive(DropDownTimerHandle) || GetWorldTimerManager().IsTimerActive(DropDownCoolDownTimerHandle))
	{
		return;
	}

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, DropDownMinimumHeightValue);

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, this);

	bool bHit = GetWorld()->LineTraceSingleByProfile(
		HitResult,
		StartLocation,
		EndLocation,
		CP_STATIC_ONLY,
		CollisionQueryParams
	);
	
	if(bIsFlying && !bHit)
	{
		RootMesh->SetSimulatePhysics(false);
		// TODO: 파일드라이버 공중 정지 시간 데이터 에셋으로 빼기
		GetWorldTimerManager().SetTimer(DropDownTimerHandle, this, &AKWPlayerCharacter::FD_ProceedAction, 0.01f, false, 0.2f);
	}
}

void AKWPlayerCharacter::TogglePauseWidget()
{
	PlayerWidgetController->TogglePauseWidget();
}

void AKWPlayerCharacter::VelocityDecelerateTimer()
{
	if(bIsReBounding || bIsKnockBackOnGoing || bIsAccelerated)
	{
		return;
	}
	
	if(static_cast<uint8>(CurrentGearState) > static_cast<uint8>(EGearState::GearTwo))
	{
		CurrentGearState = EGearState::GearTwo;
	}

	if(bIsAttackOnGoing)
	{
		RootMesh->AddForce(RootMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -10000 * (1 + DA_DecelerateValue));
	}
	else
	{
		RootMesh->AddForce(RootMesh->GetPhysicsLinearVelocity().GetSafeNormal() * -10000);
	}
	
	if(RootMesh->GetPhysicsLinearVelocity().Size2D() < 100.f)
	{
		FVector DecelerateVector = FVector(10.f, 10.f, 0.f) * RootMesh->GetPhysicsLinearVelocity().GetSafeNormal2D();
		RootMesh->SetPhysicsLinearVelocity(RootMesh->GetPhysicsLinearVelocity() - DecelerateVector);
	}
	
	if(abs(RootMesh->GetPhysicsLinearVelocity().X) <= abs(VelocityDecelerateTarget.X) && abs(RootMesh->GetPhysicsLinearVelocity().Y) <= abs(VelocityDecelerateTarget.Y))
	{
		if(bIsAttackOnGoing)
		{
			bIsAttackOnGoing = false;
		}
		VelocityDecelerateTarget = FVector::ZeroVector;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Decelerate End")));
	}
		
	if(!bIsRolling || (!bIsAttackOnGoing && bIsMoving) || abs(RootMesh->GetPhysicsLinearVelocity().Z) > 20.f)
	{
		if(bIsAttackOnGoing)
		{
			bIsAttackOnGoing = false;
		}

		VelocityDecelerateTarget = FVector::ZeroVector;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Decelerate End")));
		if(!GetWorldTimerManager().IsTimerActive(RBD_FailedTimerHandle))
		{
			GetWorldTimerManager().SetTimer(CheckIdleStateTimerHandle, this, &AKWPlayerCharacter::CheckIdleStateWhenRolling, 0.01f, true);
		}
	}
}

void AKWPlayerCharacter::	EnhancedVelocityDecelerateSequence()
{
	if (bIsAttackOnGoing || bIsMoving)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Decelerate Canceled")));
		OnDecelerationEnd(false);
		return;
	}
	
	if(bIsReBounding || bIsKnockBackOnGoing || bIsAccelerated)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Decelerate Canceled(Reset Velocity)")));
		OnDecelerationEnd();
		return;
	}
	
	VelocityDecelerateTime += GetWorld()->GetDeltaSeconds();
	float Value = VelocityDecelerateTime / CharacterData->DecelerateTime;
	
	RootMesh->SetPhysicsLinearVelocity(UKismetMathLibrary::VEase(RootMesh->GetPhysicsLinearVelocity(), FVector::Zero(), Value, CharacterData->DecelerateEasingFunction));
	
	if (Value >= 1.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f, %f"), VelocityDecelerateTime, CharacterData->DecelerateTime));
		OnDecelerationEnd();
		return;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Decelerate For Next Tick")));
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWPlayerCharacter::EnhancedVelocityDecelerateSequence);
}

void AKWPlayerCharacter::ToggleCharacterType()
{
	if(bIsFlying || bIsAttackOnGoing || bIsReBounding || bIsKnockBackOnGoing)
	{
		return;
	}
	
	if(bIsRolling) // 걷기 상태 전환
	{
		bIsRolling = false;
		GetWorldTimerManager().ClearTimer(CheckIdleStateTimerHandle);
		GetWorldTimerManager().ClearTimer(CheckGearStateTimerHandle);
		GetWorldTimerManager().ClearTimer(DA_DurationTimerHandle);
		
		RootMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootMesh->SetRelativeRotation(FRotator::ZeroRotator);

		RollingMeshComponent->SetWorldScale3D(FVector::ZeroVector);
		RollingMeshComponent->SetCollisionEnabled(::ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerComponent->SetWorldScale3D(FVector::OneVector);
		PlayerComponent->SetWorldLocation(RootMesh->GetComponentToWorld().GetLocation());

		RollingModeNiagaraComponent->SetAsset(nullptr);
		RollingModeNiagaraComponent->Deactivate();
		CurrentGearState = EGearState::KiwiMode;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("현재 걷기 상태로 전환")));
		
	}
	else // 구르기 상태 전환
	{
		bIsRolling = true;
		RootMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootMesh->SetWorldLocation(PlayerComponent->GetComponentToWorld().GetLocation());

		RollingMeshComponent->SetWorldScale3D(MeshOriginScale);
		RollingMeshComponent->SetCollisionEnabled(::ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerComponent->SetWorldScale3D(FVector::ZeroVector);

		KiwiModeNiagaraComponent->SetAsset(nullptr);
		KiwiModeNiagaraComponent->Deactivate();
		CurrentGearState = EGearState::GearOne;
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("현재 구르기 상태로 전환")));
	}
}

void AKWPlayerCharacter::CheckIdleStateWhenRolling()
{
	if(RootMesh->GetPhysicsLinearVelocity().Size() < 50.f && bIsRolling && !bIsMoving && bIsRollingIdleToWalk)
	{
		ToggleCharacterType();
		GetWorldTimerManager().ClearTimer(CheckGearStateTimerHandle);
	}
}

void AKWPlayerCharacter::RBD_JustTimingProceedAction()
{
	bIsInputJustAction = false;
}

void AKWPlayerCharacter::DA_ProceedAction()
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("일반 대시 시작")));

	bIsAttackOnGoing = true;
	AttackInputGearState = CurrentGearState;
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
	// AD_Direction.Z = RootMesh->GetPhysicsLinearVelocity().Z;
	AD_Direction.Z = 0.f;
	
	VelocityDecelerateTarget = RootMesh->GetPhysicsLinearVelocity().GetSafeNormal() * CurrentMaxVelocityValue;
	VelocityDecelerateTarget.Z = 0.f;
	if(VelocityDecelerateTarget.Length() < 100.f)
	{
		VelocityDecelerateTarget = FVector(100.f, 100.f, 0.f);
	}
	
	RootMesh->SetWorldLocation(RootMesh->GetComponentLocation() + FVector(0.f, 0.f, 30.f));
	RootMesh->SetPhysicsLinearVelocity(AD_Direction);

	UGameplayStatics::PlaySound2D(GetWorld(), GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSoundData()->KiwiDashMoveSoundCue[static_cast<int>(AttackInputGearState) + 1]);
	
	DA_Params.AddIgnoredActor(this);
	DA_ElapsedTime = 0;
	GetWorldTimerManager().SetTimerForNextTick(this,& AKWPlayerCharacter::DA_HitCheckSequence);
}

void AKWPlayerCharacter::FD_ProceedAction()
{
	DropDownElapsedTime = 0;
	RootMesh->SetSimulatePhysics(true);
	const FVector DroppingVelocity = FVector(0.f, 0.f, -DropDownVelocityValue);
	RootMesh->SetPhysicsLinearVelocity(DroppingVelocity);
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWPlayerCharacter::FD_HitCheckSequence);
	GetWorldTimerManager().SetTimer(DropDownCoolDownTimerHandle, this, &AKWPlayerCharacter::DropDownCoolDownTimer, DropDownCoolDownTime, false);
}

void AKWPlayerCharacter::CheckGearState()
{
	if(bIsAttackOnGoing)
	{
		return;
	}
	
	if(!bIsRolling)
	{
		GetWorldTimerManager().ClearTimer(CheckGearStateTimerHandle);
	}

	if(RootMesh->GetPhysicsLinearVelocity().Size2D() < 50.f && bIsRolling)
	{
		ToggleCharacterType();
		GetWorldTimerManager().ClearTimer(CheckGearStateTimerHandle);
	}
	
	float LowerGearValue = 0.f;
	if(static_cast<uint8>(CurrentGearState) - 1 >= 0)
	{
		LowerGearValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState) - 1];
	}
	
	float UpperGearValue = 0.f;
	if(static_cast<uint8>(CurrentGearState) + 1 < static_cast<uint8>(EGearState::KiwiMode))
	{
		UpperGearValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)];
	}
	
	if(LowerGearValue != 0.f && (abs(RootMesh->GetPhysicsLinearVelocity().Size2D() / 2) < LowerGearValue || abs(RootMesh->GetPhysicsLinearVelocity().X) < LowerGearValue || abs(RootMesh->GetPhysicsLinearVelocity().Y) < LowerGearValue))
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
			return;
		}
		CurrentMaxVelocityValue = DefaultMaxVelocityValue * MaxVelocityMagnificationByGear[static_cast<uint8>(CurrentGearState)];
		return;
	}
	
	if(abs(RootMesh->GetPhysicsLinearVelocity().X) > UpperGearValue || abs(RootMesh->GetPhysicsLinearVelocity().Y) > UpperGearValue)
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

	// 왜 직선 기어값 변경은 두번 체크해야 하는지 이유 불명
	if(abs(RootMesh->GetPhysicsLinearVelocity().X) > UpperGearValue || abs(RootMesh->GetPhysicsLinearVelocity().Y) > UpperGearValue)
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
		
	PlayerWidgetController->UpdateGearWidget(CurrentGearState);
}

void AKWPlayerCharacter::PlayDeadAnim()
{
	GetMesh()->GetAnimInstance()->Montage_Play(KiwiAnimMontage);
}

void AKWPlayerCharacter::StartFadeOut()
{
	GetMesh()->GetAnimInstance()->Montage_Play(KiwiAnimMontage);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SECTION_DEAD_LOOP, KiwiAnimMontage);
	GetMesh()->GetAnimInstance()->Montage_Pause(KiwiAnimMontage);
	PlayerWidgetController->StartFadeOut();
}

void AKWPlayerCharacter::RB_ApplyReBoundByObjectType(FVector& ReBoundResultValue, EReBoundObjectType ObjectType)
{
	// TODO:: 리바운드와 넉백을 구분할 수 있게 이펙트나 효과음 다르게 연출하기
	if(!bIsRolling || bIsReBounding || bIsKnockBackOnGoing)
	{
		return;
	}
	bIsAttackOnGoing = false;
	bIsReBounding = true;
	RootMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);

	ReBoundResultValue *= RB_MultiplyValuesByObjectType[static_cast<uint8>(ObjectType)];
	ReBoundResultValue *= RB_MultiplyValuesByGear[static_cast<uint8>(CurrentGearState)];
	RootMesh->SetPhysicsLinearVelocity(ReBoundResultValue);
	GetWorldTimerManager().SetTimer(RB_ContactCheckHandle, this, &AKWPlayerCharacter::RB_CheckContactToFloor, 0.01f, false, 0.3f);
}

void AKWPlayerCharacter::RB_ApplyKnockBackByObjectType(FVector& ReBoundResultValue, EReBoundObjectType ObjectType)
{
	// TODO:: 리바운드와 넉백을 구분할 수 있게 이펙트나 효과음 다르게 연출하기
	if(!bIsRolling || bIsReBounding || bIsKnockBackOnGoing)
	{
		return;
	}
	bIsAttackOnGoing = false;
	bIsKnockBackOnGoing = true;
	RootMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);

	ReBoundResultValue *= RB_MultiplyValuesByObjectType[static_cast<uint8>(ObjectType)];
	ReBoundResultValue *= RB_MultiplyValuesByGear[static_cast<uint8>(CurrentGearState)];
	RootMesh->SetPhysicsLinearVelocity(ReBoundResultValue);
	GetWorldTimerManager().SetTimer(RB_ContactCheckHandle, this, &AKWPlayerCharacter::RB_CheckContactToFloor, 0.01f, false, 0.3f);
}

void AKWPlayerCharacter::RB_CheckContactToFloor()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
		
	bool bResult = GetWorld()->SweepSingleByProfile(
	HitResult,
	GetActorLocation(),
	GetActorLocation() - FVector(0.f, 0.f, 80.f),
	FQuat::Identity,
	CP_PLAYER,
	FCollisionShape::MakeBox(FVector(50.f, 50.f, 1.f)),
	Params);

	if(bResult)
	{
		if(GetWorldTimerManager().IsTimerActive(RBD_JustTimingCheckHandle))
		{
			GetWorldTimerManager().ClearTimer(RBD_JustTimingCheckHandle);
		}
		
		if(bIsKnockBackOnGoing)
		{
			bIsKnockBackOnGoing = false;
			return;
		}
		
		if(bIsInputJustAction)
		{
			bIsReBounding = false;
			bIsInputJustAction = false;
			UGameplayStatics::PlaySound2D(GetWorld(), GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSoundData()->KiwiReBoundSuccessSoundCue);
			RBD_SuccessEvent();
			return;
		}
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("리바운드 패널티 적용")));
		DisableInput(Cast<APlayerController>(GetController()));
		if(GetWorldTimerManager().IsTimerActive(CheckIdleStateTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(CheckIdleStateTimerHandle);
		}
		UGameplayStatics::PlaySound2D(GetWorld(), GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSoundData()->KiwiReBoundFailedSoundCue);
		GetWorldTimerManager().SetTimer(RBD_FailedTimerHandle, this, &AKWPlayerCharacter::RBD_FailedPenaltyEndEvent, 0.01f, false, RB_DisableMovementTime);
		return;
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWPlayerCharacter::RB_CheckContactToFloor);
}

void AKWPlayerCharacter::RBD_SuccessEvent()
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("리바운드 대시 시작")));
	
	// 리바운드 대쉬 실행
	bIsAttackOnGoing = true;
	AttackInputGearState = EGearState::GearFour;
	CurrentGearState = EGearState::GearFour;
	
	FVector2D MousePosition;
	int ScreenSizeX;
	int ScreenSizeY;
		
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);
		
	const FVector MousePosition3D = FVector(-MousePosition.Y, MousePosition.X, 0.f);
	const FVector ScreenCenter3D = FVector(-ScreenSizeY / 2, ScreenSizeX / 2, 0.0f);
	FVector AD_Direction = (MousePosition3D - ScreenCenter3D).GetSafeNormal() * RBD_AddVelocityValue;
	AD_Direction.Z = 0.f;
	
	RootMesh->SetWorldLocation(RootMesh->GetComponentLocation() + FVector(0.f, 0.f, 30.f));
	VelocityDecelerateTarget = RootMesh->GetPhysicsLinearVelocity().GetSafeNormal() * CurrentMaxVelocityValue;
	RootMesh->SetPhysicsLinearVelocity(AD_Direction);

	AttackEffectNiagaraComponent->ResetSystem();
	AttackEffectNiagaraComponent->SetAsset(CharacterData->DA_MoveNiagaraSystem);
	AttackEffectNiagaraComponent->Activate();
	
	UGameplayStatics::PlaySound2D(GetWorld(), GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSoundData()->KiwiDashMoveSoundCue[static_cast<int>(CurrentGearState)]);
	GetWorldTimerManager().SetTimerForNextTick(this,& AKWPlayerCharacter::DA_HitCheckSequence);
}

void AKWPlayerCharacter::RBD_FailedPenaltyEndEvent()
{
	bIsReBounding = false;
	EnableInput(Cast<APlayerController>(GetController()));
}

void AKWPlayerCharacter::DA_HitCheckSequence()
{
	DA_ElapsedTime += GetWorld()->DeltaTimeSeconds;
	if(DA_ElapsedTime >= DA_DurationTime)
	{
		AttackEffectNiagaraComponent->Deactivate();
		DA_ElapsedTime = 0;
		CurrentGearState = EGearState::GearTwo;
		DA_Params.ClearIgnoredSourceObjects();
		GetWorldTimerManager().SetTimer(AttackCoolDownTimerHandle, this, &AKWPlayerCharacter::AttackCoolDownTimer, AttackCoolDownTime, false);
		if(!bIsDecelerateOnGoing)
		{
			BeginDeceleration();
			return;
		}
	}
	
	FHitResult HitResult;
	// TODO: 히트 박스 범위 데이터 에셋으로 받기
	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation(),
	FQuat::Identity,
	ECC_ENEMY_ONLY,
	FCollisionShape::MakeSphere(90.0f),
	DA_Params);

	if(bIsEnableHitCheckDebugView)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 90.f, 32, FColor::Magenta, false, 0.3f);
	}
	
	if(bResult)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("충돌 대상에게 데미지 적용")));

		if(!EventNiagaraComponent->IsActive())
		{
			EventNiagaraComponent->ResetSystem();
			EventNiagaraComponent->SetWorldRotation((HitResult.GetActor()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation());
			EventNiagaraComponent->AddWorldRotation(FRotator(0.f, -90.f, 0.f));
			EventNiagaraComponent->SetAsset(CharacterData->DA_AttackNiagaraSystem);
			EventNiagaraComponent->Activate();
		}
		
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(DA_BaseDamage * DA_MultiplyDamageByGear[static_cast<uint8>(AttackInputGearState)], DamageEvent, GetController(), this);
		FVector CollisionDirection = (GetActorLocation() - HitResult.GetActor()->GetActorLocation()).GetSafeNormal();
		CollisionDirection.Z = 1.5f;
		CollisionDirection *= 1000.f;
		RB_ApplyReBoundByObjectType(CollisionDirection, EReBoundObjectType::Enemy);
		DA_Params.AddIgnoredActor(HitResult.GetActor());
	}
	GetWorldTimerManager().SetTimerForNextTick(this,& AKWPlayerCharacter::DA_HitCheckSequence);
}

void AKWPlayerCharacter::FD_HitCheckSequence()
{
	IsDropDownActive = true;
	FHitResult GroundResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	// TODO: 히트 박스 범위 데이터 에셋으로 받기
	bool GroundCheck = GetWorld()->SweepSingleByProfile(
	GroundResult,
	GetActorLocation(),
	GetActorLocation() - FVector(0.f, 0.f, 90.f),
	FQuat::Identity,
	CP_PLAYER,
	FCollisionShape::MakeBox(FVector(10.f, 10.f, 5.f)),
	Params);

	if(GroundCheck)
	{
		IsDropDownActive = false;
		// TODO: 바닥에 충격파 히트 체크및 이펙트
		return;
	}
	
	FHitResult HitResult;
	// TODO: 히트 박스 범위 데이터 에셋으로 받기
	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation() - FVector(0.f, 0.f, 10.f),
	FQuat::Identity,
	ECC_ENEMY_ONLY,
	FCollisionShape::MakeSphere(90.0f),
	FD_Params);

	if(bIsEnableHitCheckDebugView)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 90.f, 32, FColor::Magenta, false, 0.3f);
	}	
	
	if(bResult)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("충돌 대상에게 데미지 적용")));
		
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(DropDownDamage, DamageEvent, GetController(), this);
		FD_Params.AddIgnoredActor(HitResult.GetActor());
	}
	GetWorldTimerManager().SetTimerForNextTick(this,& AKWPlayerCharacter::FD_HitCheckSequence);
}

void AKWPlayerCharacter::AttackCoolDownTimer()
{
	// Do SomeThing;
}

void AKWPlayerCharacter::DropDownCoolDownTimer()
{
	// Do SomeThing
}

void AKWPlayerCharacter::BeginDeceleration()
{
	bIsDecelerateOnGoing = true;
	VelocityDecelerateTime = 0.f;
	VelocityDecelerateTarget = FVector::Zero();
	
	if(static_cast<uint8>(CurrentGearState) > static_cast<uint8>(EGearState::GearTwo))
	{
		CurrentGearState = EGearState::GearTwo;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Decelerate Start")));
	GetWorldTimerManager().SetTimerForNextTick(this, &AKWPlayerCharacter::EnhancedVelocityDecelerateSequence);
	// FTimerHandle DecelerationTimerHandle;
	//GetWorldTimerManager().SetTimer(DecelerationTimerHandle, this, &AKWPlayerCharacter::EnhancedVelocityDecelerateSequence, GetWorld()->GetDeltaSeconds(), false);
}

void AKWPlayerCharacter::OnDecelerationEnd(bool ResetVelocity)
{
	bIsDecelerateOnGoing = false;
	VelocityDecelerateTime = 0.f;
	if (ResetVelocity)
	{
		RootMesh->SetPhysicsLinearVelocity(VelocityDecelerateTarget);
	}
	
	if(!bIsRolling || (!bIsAttackOnGoing && bIsMoving) || abs(RootMesh->GetPhysicsLinearVelocity().Z) > 20.f)
	{
		if(bIsAttackOnGoing)
		{
			bIsAttackOnGoing = false;
		}

		VelocityDecelerateTarget = FVector::Zero();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Decelerate End")));
		if(!GetWorldTimerManager().IsTimerActive(RBD_FailedTimerHandle))
		{
			GetWorldTimerManager().SetTimer(CheckIdleStateTimerHandle, this, &AKWPlayerCharacter::CheckIdleStateWhenRolling, 0.01f, true);
		}
	}
}

void AKWPlayerCharacter::DamageDelayTimer()
{
	// Do SomeThing
}

void AKWPlayerCharacter::SetAccelerate()
{
	bIsAccelerated = true;
	GetWorldTimerManager().SetTimer(AccelerateTimerHandle, this, &AKWPlayerCharacter::AccelerationTimer, 0.5f, false);
}

void AKWPlayerCharacter::AccelerationTimer()
{
	bIsAccelerated = false;
	// Do SomeThing
}



