// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "KWSoundData.generated.h"

/**
 * 게임에 적용되는 전반적인 사운드 데이터를 저장하는 데이터 에셋 \n
 * 다만 UI의 경우에는 BP에서 직접 지정하도록 한다.
 **/
UCLASS(BlueprintType)
class XR_PROJECT_TEAM10_API UKWSoundData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Player, DisplayName = "플레이어 대시 돌진 사운드")
	TArray<USoundCue*> KiwiDashMoveSoundCue;

	UPROPERTY(EditAnywhere, Category = Player, DisplayName = "플레이어 대시 타격 사운드")
	TObjectPtr<USoundCue> KiwiDashHitSoundCue;
	
	UPROPERTY(EditAnywhere, Category = Player, DisplayName = "플레이어 리바운드 성공 사운드")
	TObjectPtr<USoundCue> KiwiReBoundSuccessSoundCue;
	
	UPROPERTY(EditAnywhere, Category = Player, DisplayName = "플레이어 리바운드 실패 사운드")
	TObjectPtr<USoundCue> KiwiReBoundFailedSoundCue;
	
public:
	UPROPERTY(EditAnywhere, Category = Gimmick, DisplayName = "가속 장치 사운드")
	TObjectPtr<USoundCue> BoostPadSoundCue;

	UPROPERTY(EditAnywhere, Category = Gimmick, DisplayName = "리바운드 장치 사운드")
	TObjectPtr<USoundCue> TrapPadSoundCue;

	UPROPERTY(EditAnywhere, Category = Gimmick, DisplayName = "스프링 사운드")
	TObjectPtr<USoundCue> SpringGimmickSoundCue;

public:
	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "로비 배경음악")
	TObjectPtr<USoundCue> LobbyBGMSoundCue;

	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "인트로 배경음악")
	TObjectPtr<USoundCue> IntroBGMSoundCue;
	
	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "기본 배경음악")
	TObjectPtr<USoundCue> FieldBGMSoundCue;
	
	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "전투 배경음악")
	TObjectPtr<USoundCue> BattleBGMSoundCue;
	
	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "보스 배경음악")
	TObjectPtr<USoundCue> BossBGMSoundCue;

	UPROPERTY(EditAnywhere, Category = BGM, DisplayName = "사망 배경음악")
	TObjectPtr<USoundCue> DeadBGMSoundCue;
	
};
