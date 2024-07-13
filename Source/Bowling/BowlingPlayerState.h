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
	// TODO: Score
	// Scoring is based on the number of pins knocked down.
	// Except, when you get a spare, you get 10 plus the number of pins you knock down during your next throw.
	// If you get a strike, you get 10 plus the number of pins you knock down with your next two throws. 
};

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
};
