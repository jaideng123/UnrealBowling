// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Bowling/BowlingPlayerState.h"
#include "BowlingScoreCard.generated.h"

class ABowlingGameStateBase;
class UVerticalBox;
class UBowlingScoreCardRow;
/**
 * Score card the shows all player's scores
 */
UCLASS()
class BOWLING_API UBowlingScoreCard : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RowContainer;

	UFUNCTION()
	void OnPlayerStateAdded(ABowlingPlayerState* PlayerState);
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnPlayerStateRemoved(int32 PlayerId);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBowlingScoreCardRow> RowTemplate;
	// Rows mapped from player id
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TMap<int32, UBowlingScoreCardRow*> Rows;
public:
	UFUNCTION(BlueprintCallable)
	void SyncWithPlayerState(ABowlingPlayerState* PlayerState);
};
