// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundCue.h"
#include "PPSoundData.generated.h"

/**
 * 게임에 적용되는 전반적인 사운드 데이터를 저장하는 데이터 에셋 \n
 * 다만 UI의 경우에는 BP에서 직접 지정하도록 한다.
 **/
UCLASS()
class XR_PROJECT_TEAM10_API UPPSoundData : public UDataAsset
{
	GENERATED_BODY()

};
