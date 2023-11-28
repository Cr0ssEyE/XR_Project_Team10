// Fill out your copyright notice in the Description page of Project Settings.


#include "XR_Project_Team10/Game/KWGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "XR_Project_Team10/Util/PPConstructorHelper.h"

UKWGameInstance::UKWGameInstance()
{
	SaveSettingOption = CastChecked<UKWSaveSettingOption>(UGameplayStatics::CreateSaveGameObject(UKWSaveSettingOption::StaticClass()));
	TObjectPtr<UKWSaveSettingOption> SavedOption = Cast<UKWSaveSettingOption>(UGameplayStatics::LoadGameFromSlot(SaveSettingOption->SaveFileName, 0));
	if(SavedOption)
	{
		SaveSettingOption = SavedOption;
	}
	
	// StringDataTable = FPPConstructorHelper::FindAndGetObject<UDataTable>(TEXT("/Script/Engine.DataTable'/Game/Rolling-Kiwi/DataTable/StringData.StringData'"), EAssertionLevel::Check);

	SoundData = FPPConstructorHelper::FindAndGetObject<UKWSoundData>(TEXT("/Script/XR_Project_Team10.KWSoundData'/Game/Rolling-Kiwi/Datas/DataAssets/SoundData.SoundData'"), EAssertionLevel::Check);

	bIsSavePointActivated = false;
	bIsMainLevelFirstEnter = false;
	bIsBossVisited = false;
	SavePointsArrayNum = -1;
}

FStringDataTable* UKWGameInstance::GetStringDataTable(const FName RowName)
{
	return StringDataTable->FindRow<FStringDataTable>(RowName, TEXT("Find StringDataTable"));
}
