// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameUserSettings.h"

UBowlingGameUserSettings::UBowlingGameUserSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	MusicVolumeSetting = 1;
}

void UBowlingGameUserSettings::SetMusicVolume(float NewVolume)
{
	MusicVolumeSetting = NewVolume;
}

float UBowlingGameUserSettings::GetMusicVolume() const
{
	return MusicVolumeSetting;
}

UBowlingGameUserSettings* UBowlingGameUserSettings::GetBowlingGameUserSettings()
{
	return Cast<UBowlingGameUserSettings>(UBowlingGameUserSettings::GetGameUserSettings());
}
