// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingScoreCardFinalEntry.generated.h"

class UTextBlock;
/**
 * The Final Entry of a player's score card, supports up to 3 balls instead of the usual 2
 */
UCLASS()
class BOWLING_API UBowlingScoreCardFinalEntry : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ball1Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ball2Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ball3Text;
	virtual void NativeConstruct() override;
public:
	void SetScore(int I);
	void SetBall1(int ball1);
	void SetBall2(int ball2, bool wasSpare);
	void SetBall3(int ball3, bool wasSpare);
};
