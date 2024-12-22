// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

	UPROPERTY(EditDefaultsOnly)
	TArray<UWidget*> DesktopOnlyWidgets;

	UPROPERTY(EditDefaultsOnly)
	TArray<UWidget*> MobileOnlyWidgets;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName TitleMapUrl;
	
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void ReturnToTitleScreen();
};
