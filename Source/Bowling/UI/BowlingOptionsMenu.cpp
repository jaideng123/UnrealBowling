// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingOptionsMenu.h"

#include "Bowling/BowlingGameUserSettings.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UBowlingOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if(PlatformName.Equals("Windows", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Mac", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Linux", ESearchCase::IgnoreCase))
	{
		ResolutionContainer->SetVisibility(ESlateVisibility::Visible);
		WindowModeContainer->SetVisibility(ESlateVisibility::Visible);

		MotionControlToggleContainer->SetVisibility(ESlateVisibility::Collapsed);
		// FScreenResolutionArray Resolutions;
		// RHIGetAvailableResolutions(Resolutions, false);
		// for(const auto& Resolution : Resolutions)
		// {
		// 	
		// }
	}
	else if(PlatformName.Equals("IOS", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Android", ESearchCase::IgnoreCase))
	{
		ResolutionContainer->SetVisibility(ESlateVisibility::Collapsed);
		WindowModeContainer->SetVisibility(ESlateVisibility::Collapsed);

		MotionControlToggleContainer->SetVisibility(ESlateVisibility::Visible);
	}

	// UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	// Settings->GetScreenResolution();
	// Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	// Settings->SetScreenResolution()
	// Settings->ApplyResolutionSettings(false);
	// Settings->ApplySettings(false);
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	MusicSlider->SetValue(GameUserSettings->GetMusicVolume());

	MusicSlider->OnValueChanged.AddDynamic(this,&UBowlingOptionsMenu::UpdateMusicVolume);
}

void UBowlingOptionsMenu::UpdateMusicVolume_Implementation(float value)
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	GameUserSettings->SetMusicVolume(value);
}

void UBowlingOptionsMenu::RestartGame()
{
	// TODO: reset all state instead of reopening the level
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UBowlingOptionsMenu::ReturnToTitleScreen()
{
	UGameplayStatics::OpenLevel(GetWorld(), TitleMapUrl);
}
