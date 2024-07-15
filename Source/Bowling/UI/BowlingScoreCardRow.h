// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingScoreCardRow.generated.h"

class UBowlingScoreCardEntry;
class UWrapBox;
/**
 * 
 */
UCLASS()
class BOWLING_API UBowlingScoreCardRow : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* EntryContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBowlingScoreCardEntry> EntryTemplate;

	// Entries in a row
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<UBowlingScoreCardEntry*> Entries;
};
