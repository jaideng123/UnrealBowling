// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingEndUI.generated.h"

class ABowlingPlayerState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingEndUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinningScoreText;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName TitleMapUrl;
	
	UFUNCTION(BlueprintCallable)
	void RestartGame();
	
	UFUNCTION(BlueprintCallable)
	void ReturnToTitleScreen();

	UFUNCTION()
	void SyncWithPlayerState(ABowlingPlayerState* playerState);
};
