// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingEndUI.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingEndUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName TitleMapUrl;
	
	UFUNCTION(BlueprintCallable)
	void RestartGame();
	
	UFUNCTION(BlueprintCallable)
	void ReturnToTitleScreen();
};
