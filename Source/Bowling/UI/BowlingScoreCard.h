// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingScoreCard.generated.h"

class UBowlingScoreCardRow;
/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingScoreCard : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBowlingScoreCardRow> RowTemplate;
	// Rows mapped from player id
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TMap<int32, UBowlingScoreCardRow*> Rows;
};
