// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BowlingScoreCardRow.generated.h"

class UScrollBox;
struct FBowlingFrame;
class ABowlingPlayerState;
class UBowlingScoreCardEntry;
class UBowlingScoreCardFinalEntry;
class UWrapBox;
/**
 * Class representing a single player's portion of the score card
 */
UCLASS()
class BOWLING_API UBowlingScoreCardRow : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* EntryContainer;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBowlingScoreCardEntry> EntryTemplate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBowlingScoreCardFinalEntry> FinalEntryTemplate;

	// Entries in a row
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<UBowlingScoreCardEntry*> Entries;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	UBowlingScoreCardFinalEntry* FinalEntry;
	
public:
	void SyncWithPlayerState(TObjectPtr<ABowlingPlayerState> playerState);
};
