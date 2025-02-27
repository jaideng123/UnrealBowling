﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "BowlingGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float NewVolume);

	UFUNCTION(BlueprintCallable)
	float GetMusicVolume() const;

	UFUNCTION(BlueprintCallable)
	void SetSoundEffectVolume(float NewVolume);

	UFUNCTION(BlueprintCallable)
	float GetSoundEffectVolume() const;

	UFUNCTION(BlueprintCallable)
	void SetMotionControlsEnabled(bool NewValue);

	UFUNCTION(BlueprintCallable)
	bool GetMotionControlsEnabled() const;

	UFUNCTION(BlueprintCallable)
	static UBowlingGameUserSettings* GetBowlingGameUserSettings();

protected:
	UPROPERTY(Config)
	float MusicVolumeSetting;

	UPROPERTY(Config)
	float SoundEffectVolumeSetting;

	UPROPERTY(Config)
	bool MotionControlsEnabledSetting;
};
