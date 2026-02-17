// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingTitleScreen.generated.h"

UENUM(BlueprintType)
enum EAndroidHapticFeedbackConstant
{
	None = 0xffffffff,
	Long_Press = 0,
	Virtual_Key = 1,
	Keyboard_Press = 3,
	Clock_Tick = 4,
	Context_Click = 6,
	Confirm = 16,
	Reject = 17,
};

/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingTitleScreen : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName GameplayMapUrl;

	UFUNCTION(BlueprintCallable)
	void OnStartGame(int numPlayers);

	UFUNCTION(BlueprintCallable)
	void OnQuitGame();

	UFUNCTION(BlueprintCallable)
	void PlayHaptic(TEnumAsByte<EAndroidHapticFeedbackConstant> HapticType);
};
