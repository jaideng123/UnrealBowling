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
	UFUNCTION(CallInEditor, BlueprintCallable)
	void TestPins();
	
	UPROPERTY(Replicated, VisibleAnywhere, ReplicatedUsing = OnRep_Frames)
	TArray<FBowlingFrame> Frames;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int CurrentFrame = 0;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int CurrentBall = 0;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerScoreChanged OnScoreChangedDelegate;

	UFUNCTION(Reliable, NetMulticast )
	void MulticastScoreChanged();

	UFUNCTION()
	void OnRep_Frames();
private:
	void RecalculateScore(const TArray<FBowlingFrame>& Array) const;
	void ReportStrikeOrSpare();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
