// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "TouchTimer.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UTouchTimer : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateTimerProgress(float Progress);
	UFUNCTION(BlueprintCallable)
	void UpdatePosition(FVector2D Position);

	UPROPERTY(meta = (BindWidget))
	UWidget* TimerContainer;
};
