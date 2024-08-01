// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingTitleScreen.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingTitleScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName GameplayMapUrl;

	UFUNCTION(BlueprintCallable)
	void OnStartGame(int numPlayers);

	UFUNCTION(BlueprintCallable)
	void OnQuitGame();
};
