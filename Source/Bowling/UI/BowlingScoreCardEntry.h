// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingScoreCardEntry.generated.h"

class UTextBlock;
/**
 * An entry of a score card showing a single frame of bowling
 */
UCLASS()
class BOWLING_API UBowlingScoreCardEntry : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ball1Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ball2Text;
	void NativeConstruct() override;
public:
	void SetScore(int I);
	void SetBall1(int ball1);
	void SetBall2(int ball2);
};
