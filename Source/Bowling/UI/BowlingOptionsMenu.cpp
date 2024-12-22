// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingOptionsMenu.h"

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
		for(auto Widget : DesktopOnlyWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
		for(auto Widget : MobileOnlyWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
		FScreenResolutionArray Resolutions;
		RHIGetAvailableResolutions(Resolutions, false);
		for(const auto& Resolution : Resolutions)
		{
			
		}
		
		UGameUserSettings* Settings = GEngine->GetGameUserSettings();
		// Settings->GetScreenResolution();
		// Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		// Settings->SetScreenResolution()
		// Settings->ApplyResolutionSettings(false);
		// Settings->ApplySettings(false);
		
	}
	else if(PlatformName.Equals("IOS", ESearchCase::IgnoreCase) ||
		PlatformName.Equals("Android", ESearchCase::IgnoreCase))
	{
		for(auto Widget : DesktopOnlyWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
		for(auto Widget : MobileOnlyWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
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
