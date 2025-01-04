// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardFinalEntry.h"

#include "Bowling/BowlingGameModeBase.h"
#include "Components/TextBlock.h"


void UBowlingScoreCardFinalEntry::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreText->SetText(FText::FromString(TEXT("")));
	Ball1Text->SetText(FText::FromString(TEXT("")));
	Ball2Text->SetText(FText::FromString(TEXT("")));
	Ball3Text->SetText(FText::FromString(TEXT("")));
}

void UBowlingScoreCardFinalEntry::SetScore(int score)
{
	if(score < 0)
	{
		ScoreText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}
}

void UBowlingScoreCardFinalEntry::SetBall1(int ball1)
{
	if(ball1 < 0)
	{
		Ball1Text->SetText(FText::FromString(TEXT("")));
	}
	else if(ball1 == ABowlingGameModeBase::GetNumPins(GetWorld()))
	{
		Ball1Text->SetText(FText::FromString(TEXT("X")));
	}
	else
	{
		Ball1Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), ball1)));
	}
}

void UBowlingScoreCardFinalEntry::SetBall2(int ball2, bool wasSpare)
{
	if(ball2 < 0)
	{
		Ball2Text->SetText(FText::FromString(TEXT("")));
	}
	else if(wasSpare)
	{
		Ball2Text->SetText(FText::FromString(TEXT("/")));
	}
	else if(ball2 == ABowlingGameModeBase::GetNumPins(GetWorld()))
	{
		Ball2Text->SetText(FText::FromString(TEXT("X")));
	}
	else
	{
		Ball2Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), ball2)));
	}
}

void UBowlingScoreCardFinalEntry::SetBall3(int ball3, bool wasSpare)
{
	if(ball3 < 0)
	{
		Ball3Text->SetText(FText::FromString(TEXT("")));
	}
	else if(wasSpare)
	{
		Ball3Text->SetText(FText::FromString(TEXT("/")));
	}
	else if(ball3 == ABowlingGameModeBase::GetNumPins(GetWorld()))
	{
		Ball3Text->SetText(FText::FromString(TEXT("X")));
	}
	else
	{
		Ball3Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), ball3)));
	}
}
