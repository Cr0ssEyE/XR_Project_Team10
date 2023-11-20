// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XR_Project_Team10/Util/KWSoundData.h"
#include "XR_Project_Team10/Util/KWSaveSettingOption.h"
#include "XR_Project_Team10/Util/StringDataTable.h"
#include "KWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XR_PROJECT_TEAM10_API UKWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UKWGameInstance();
	
public:
	FORCEINLINE TObjectPtr<UKWSaveSettingOption> GetSaveSettingOption() const { return SaveSettingOption; }
	FStringDataTable* GetStringDataTable(const FName RowName);
	FORCEINLINE TObjectPtr<UKWSoundData> GetSoundData() const { return SoundData; }

	FORCEINLINE bool CheckSavePointActivate() const { return bIsSavePointActivated; }
	FORCEINLINE int GetSavePointNum() const { return SavePointsArrayNum; }
	FORCEINLINE void SetSavePoint(int Num) { bIsSavePointActivated = true; SavePointsArrayNum = Num; }
	
private:
	// 인스턴스내에 환경설정 값을 저장시켜놓고 레벨을 옮길 때 레벨에서 인스턴스의 환경설정 값을 적용
	UPROPERTY()
	TObjectPtr<UKWSaveSettingOption> SaveSettingOption;

	// 튜토리얼, 자막 등에 사용 할 문자열 데이터 테이블
	UPROPERTY()
	TObjectPtr<UDataTable> StringDataTable;
	
	// 사운드 재생 및 등록에 사용 할 사운드데이터
	UPROPERTY()
	TObjectPtr<UKWSoundData> SoundData;

private:
	UPROPERTY()
	uint8 bIsSavePointActivated : 1;

	UPROPERTY()
	int SavePointsArrayNum;
	
};
