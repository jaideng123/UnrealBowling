// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardEntry.h"

#include "Bowling/BowlingGameStateBase.h"
#include "Components/TextBlock.h"


void UBowlingScoreCardEntry::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreText->SetText(FText::FromString(TEXT("")));
	Ball1Text->SetText(FText::FromString(TEXT("")));
	Ball2Text->SetText(FText::FromString(TEXT("")));
}

void UBowlingScoreCardEntry::SetFrameNumber(int I)
{
	FrameNumberText->SetText(FText::FromString(FString::Printf(TEXT("%d"), I)));
}

void UBowlingScoreCardEntry::SetScore(int score)
{
	if (score < 0)
	{
		ScoreText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}
}

void UBowlingScoreCardEntry::SetBall1(int ball1)
{
	if (ball1 < 0)
	{
		Ball1Text->SetText(FText::FromString(TEXT("")));
	}
	else if (ball1 == ABowlingGameStateBase::GetNumPins(GetWorld()))
	{
		Ball1Text->SetText(FText::FromString(TEXT("X")));
	}
	else
	{
		Ball1Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), ball1)));
	}
}

void UBowlingScoreCardEntry::SetBall2(int ball2, bool wasSpare)
{
	if (ball2 < 0)
	{
		Ball2Text->SetText(FText::FromString(TEXT("")));
	}
	else if (wasSpare)
	{
		Ball2Text->SetText(FText::FromString(TEXT("/")));
	}
	else
	{
		Ball2Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), ball2)));
	}
}
