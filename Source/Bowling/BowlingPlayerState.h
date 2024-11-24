// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BowlingPlayerState.generated.h"

// A bowling frame is a record of the scoring that occurred during a bowlers turn
USTRUCT()
struct FBowlingFrame {
	GENERATED_BODY()
public:
	UPROPERTY(VisibleInstanceOnly)
	int ball1Pins = -1;
	UPROPERTY(VisibleInstanceOnly)
	int ball2Pins = -1;
	UPROPERTY(VisibleInstanceOnly)
	int ball3Pins = -1;
	UPROPERTY(VisibleInstanceOnly)
	mutable int score = -1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreChanged, ABowlingPlayerState*, PlayerState);

/**
 * 
 */
UCLASS()
class BOWLING_API ABowlingPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ReportPins(int numPins);
	UFUNCTION(CallInEditor)
	void TestPins();
	
	UPROPERTY(VisibleAnywhere)
	TArray<FBowlingFrame> Frames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentFrame = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentBall = 0;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerScoreChanged OnScoreChangedDelegate;
private:
	void RecalculateScore(const TArray<FBowlingFrame>& Array) const;
};
