// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardEntry.h"

#include "Components/TextBlock.h"


void UBowlingScoreCardEntry::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreText->SetText(FText::FromString(TEXT("")));
	Ball1Text->SetText(FText::FromString(TEXT("")));
	Ball2Text->SetText(FText::FromString(TEXT("")));
}

void UBowlingScoreCardEntry::SetScore(int score)
{
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
}
