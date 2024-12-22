// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "BowlingOptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingOptionsMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ToggleMotionControlsText;

	UPROPERTY(meta = (BindWidget))
	UWidget* ResolutionContainer;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* WindowModeContainer;

	UPROPERTY(meta = (BindWidget))
	UWidget* MotionControlToggleContainer;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MotionControlToggleTextSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* ToggleMotionControlsButton;

	UPROPERTY(meta = (BindWidget))
	USlider* MusicSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* SoundEffectSlider;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionComboBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ScreenModeComboBox;
	

	UPROPERTY(EditAnywhere)
	TArray<UWidget*> MobileOnlyWidgets;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName TitleMapUrl;

	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMusicVolume(float value);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateSoundEffectVolume(float value);

	UFUNCTION()
	void ToggleMotionControls();
	
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void ReturnToTitleScreen();

	UFUNCTION(BlueprintCallable)
	void OnClose();
};
