#include "XR_Project_Team10/Interaction/JumpPad.h"

#include "XR_Project_Team10/Constant/KWCollisionChannel.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;

	JumpAnim = FPPConstructorHelper::FindAndGetObject<UAnimSequence>(TEXT("/Script/Engine.AnimSequence'/Game/1-Graphic-Resource/Props/WithGimmick/Jump_Mushroom/Animation/AS_Mushroom_Bounce'"));

	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetSkeletalMesh(FPPConstructorHelper::FindAndGetObject<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/1-Graphic-Resource/Props/WithGimmick/Jump_Mushroom/Animation/SKM_Mushroom'")));
	RootComponent = BaseMesh;
	BaseMesh->SetWorldScale3D(FVector(30.f, 30.f, 30.f));
	BaseMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(BaseMesh);
	CollisionBox->SetBoxExtent(FVector(2.f, 2.f, 3.f));
	CollisionBox->SetCollisionProfileName(CP_GIMMICK);
	bResetVelocity = false;
	bIsPlayerIn = false;
}

#include "XR_Project_Team10/Player/KWPlayerCharacter.h"
void AJumpPad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = true;

		Player = PlayableCharacter;
		JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);
		
		if(bResetVelocity)
		{
			Player->GetMeshComp()->SetPhysicsLinearVelocity(FVector::ZeroVector);
		}
		switch (JumpPadType)
		{
		case EJumpPadType::E_Contact:
			ContactJump();
			break;
		case EJumpPadType::E_Timing:
			TimingJump();
			break;
		case EJumpPadType::E_Delay:
			if (!GetWorldTimerManager().IsTimerActive(JumpTimerHandle))
				GetWorldTimerManager().SetTimer(JumpTimerHandle, this, &AJumpPad::DelayJump, DelayTime, false);
			break;
		case EJumpPadType::E_PileDriver:
			PileDriverJump();
			break;
		default:
			checkNoEntry();
		}
	}
}

void AJumpPad::ContactJump()
{
	if (nullptr != Player) {
		auto PlayerMeshComp = Player->GetMeshComp();

		JumpVelocityVector += PlayerMeshComp->GetPhysicsLinearVelocity();
		PlayerMeshComp->SetPhysicsLinearVelocity(JumpVelocityVector);
		BaseMesh->PlayAnimation(JumpAnim, false);
	}
}

void AJumpPad::DelayJump() {
	if (nullptr != Player) {
		ContactJump();
	}
	else {
		GetWorldTimerManager().ClearTimer(JumpTimerHandle);
	}
}

void AJumpPad::TimingJump() {
	if (Player->IsDropDownActive){
		JumpVelocityVector.Z += JumpAddForceValue;
	}
	ContactJump();
	JumpVelocityVector = FVector(0.f, 0.f, JumpForceValue);
}

void AJumpPad::PileDriverJump()
{
	UE_LOG(LogTemp, Log, TEXT("%d"), Player->IsDropDownActive);
	if (Player->IsDropDownActive){
		UE_LOG(LogTemp, Log, TEXT("piledriver jump")); 
		ContactJump();
	}
}

void AJumpPad::NotifyActorEndOverlap(AActor* OtherActor)
{
	auto PlayableCharacter = Cast<AKWPlayerCharacter>(OtherActor);

	if (PlayableCharacter)
	{
		bIsPlayerIn = false;
		Player = nullptr;
	}
}