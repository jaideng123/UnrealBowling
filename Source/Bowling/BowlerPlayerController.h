// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BowlerPawn.h"
#include "GameFramework/PlayerController.h"
#include "BowlerPlayerController.generated.h"

class UBowlingScoreCard;
/**
 * 
 */
UCLASS()
class BOWLING_API ABowlerPlayerController : public APlayerController
{
	GENERATED_BODY()
	void                                          AttemptMoveX(float x);
	virtual void                                  BeginPlay() override;
	virtual void                                  Tick(float DeltaSeconds) override;
	void                                          HandleTouchPress(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	void                                          HandleTouchRelease(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	void                                          HandleTouchHeld(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	UE::Math::TVector<double>                     LastHoldPosition = UE::Math::TVector<double>(-1,-1,-1);
	inline static const UE::Math::TVector<double> NullPos = UE::Math::TVector<double>(-1,-1,-1);
	float                                         CurrentContinuousMove = 0.0f;
public:
	UFUNCTION(BlueprintCallable)
	void StartContinuousMove(float Direction);
	UFUNCTION(BlueprintCallable)
	void StopContinuousMove();
	UFUNCTION(BlueprintImplementableEvent)
	void HideControlUI();
	UFUNCTION(BlueprintImplementableEvent)
	void  ShowControlUI();

	UPROPERTY(BlueprintReadOnly)
	ABowlerPawn* ControlledBowler;

	UPROPERTY(BlueprintReadWrite)
	UBowlingScoreCard* ScoreCardInstance;
	
};
