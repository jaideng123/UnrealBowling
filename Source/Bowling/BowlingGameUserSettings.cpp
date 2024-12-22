// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameUserSettings.h"

UBowlingGameUserSettings::UBowlingGameUserSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	MusicVolumeSetting = 1;
	SoundEffectVolumeSetting = 1;
}

void UBowlingGameUserSettings::SetMusicVolume(float NewVolume)
{
	MusicVolumeSetting = NewVolume;
}

float UBowlingGameUserSettings::GetMusicVolume() const
{
	return MusicVolumeSetting;
}

void UBowlingGameUserSettings::SetSoundEffectVolume(float NewVolume)
{
	SoundEffectVolumeSetting = NewVolume;
}

float UBowlingGameUserSettings::GetSoundEffectVolume() const
{
	return SoundEffectVolumeSetting;
}

UBowlingGameUserSettings* UBowlingGameUserSettings::GetBowlingGameUserSettings()
{
	return Cast<UBowlingGameUserSettings>(UBowlingGameUserSettings::GetGameUserSettings());
}
