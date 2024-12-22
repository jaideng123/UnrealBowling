// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingOptionsMenu.h"

#include "Bowling/BowlingGameUserSettings.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"


void UBowlingOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();

	FString PlatformName = UGameplayStatics::GetPlatformName();
	if(PlatformName.Equals("Windows", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Mac", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Linux", ESearchCase::IgnoreCase))
	{
		ResolutionContainer->SetVisibility(ESlateVisibility::Visible);
		WindowModeContainer->SetVisibility(ESlateVisibility::Visible);

#if WITH_EDITOR
		MotionControlToggleContainer->SetVisibility(ESlateVisibility::Visible);
#else
		MotionControlToggleContainer->SetVisibility(ESlateVisibility::Collapsed);
#endif

		// FScreenResolutionArray Resolutions;
		// RHIGetAvailableResolutions(Resolutions, false);
		// for(const auto& Resolution : Resolutions)
		// {
		// 	
		// }
		TArray<FIntPoint> Resolutions;
		ResolutionComboBox->ClearOptions();
		if(UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions))
		{
			for(auto Resolution : Resolutions)
			{
				FString selection = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
				ResolutionComboBox->AddOption(FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y));
				if(Resolution == GameUserSettings->GetScreenResolution())
				{
					ResolutionComboBox->SetSelectedOption(selection);
				}
			}
		}
		ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UBowlingOptionsMenu::OnResolutionChanged);

		ScreenModeComboBox->ClearOptions();
		ScreenModeComboBox->AddOption(TEXT("Fullscreen"));
		ScreenModeComboBox->AddOption(TEXT("Windowed"));
		ScreenModeComboBox->AddOption(TEXT("Borderless"));
		if(GameUserSettings->GetFullscreenMode() == EWindowMode::Type::Fullscreen)
		{
			ScreenModeComboBox->SetSelectedOption(TEXT("Fullscreen"));
		}
		else if(GameUserSettings->GetFullscreenMode() == EWindowMode::Type::Windowed)
		{
			ScreenModeComboBox->SetSelectedOption(TEXT("Windowed"));
		}
		else if(GameUserSettings->GetFullscreenMode() == EWindowMode::Type::WindowedFullscreen)
		{
			ScreenModeComboBox->SetSelectedOption(TEXT("Borderless"));
		}
		ScreenModeComboBox->OnSelectionChanged.AddDynamic(this, &UBowlingOptionsMenu::OnScreenModeChanged);
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

	MusicSlider->SetValue(GameUserSettings->GetMusicVolume());
	MusicSlider->OnValueChanged.AddDynamic(this, &UBowlingOptionsMenu::UpdateMusicVolume);

	SoundEffectSlider->SetValue(GameUserSettings->GetSoundEffectVolume());
	SoundEffectSlider->OnValueChanged.AddDynamic(this, &UBowlingOptionsMenu::UpdateSoundEffectVolume);

	MotionControlToggleTextSwitcher->SetActiveWidgetIndex(GameUserSettings->GetMotionControlsEnabled() ? 1 : 0);
	ToggleMotionControlsButton->OnClicked.AddDynamic(this, &UBowlingOptionsMenu::ToggleMotionControls);
}

void UBowlingOptionsMenu::UpdateMusicVolume_Implementation(float value)
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	GameUserSettings->SetMusicVolume(value);
}

void UBowlingOptionsMenu::UpdateSoundEffectVolume_Implementation(float value)
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	GameUserSettings->SetSoundEffectVolume(value);
}

void UBowlingOptionsMenu::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	TArray<FString> ResultString;

	SelectedItem.TrimStartAndEndInline();
	SelectedItem.ParseIntoArray(ResultString, TEXT("x"), true);
	GameUserSettings->SetScreenResolution(FIntPoint(FCString::Atoi(*ResultString[0]), FCString::Atoi(*ResultString[1])));
	GameUserSettings->ApplyResolutionSettings(false);
}

void UBowlingOptionsMenu::OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	if(SelectedItem.Equals(TEXT("Windowed")))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::Type::Windowed);
	}
	else if(SelectedItem.Equals(TEXT("Fullscreen")))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::Type::Fullscreen);
	}
	else if(SelectedItem.Equals(TEXT("Borderless")))
	{
		GameUserSettings->SetFullscreenMode(EWindowMode::Type::WindowedFullscreen);
	}
	GameUserSettings->ApplyResolutionSettings(false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UBowlingOptionsMenu::ToggleMotionControls()
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	GameUserSettings->SetMotionControlsEnabled(!GameUserSettings->GetMotionControlsEnabled());

	MotionControlToggleTextSwitcher->SetActiveWidgetIndex(GameUserSettings->GetMotionControlsEnabled() ? 1 : 0);

	GameUserSettings->ApplySettings(false);
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

void UBowlingOptionsMenu::OnClose()
{
	UBowlingGameUserSettings* GameUserSettings = UBowlingGameUserSettings::GetBowlingGameUserSettings();
	GameUserSettings->ApplySettings(false);
}
