// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BowlingGameStateBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAdded, ABowlingPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoved, int32, PlayerId);

UENUM(BlueprintType)
enum EMatchState
{
	START,
	PRE_BOWL,
	BOWL_SETUP,
	THROWING,
	THROWN,
	OUT_OF_BOUNDS,
	BOWL_COMPLETE,
	CYCLE_BOWLER,
	GAME_END,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateExited);

UCLASS(BlueprintType)
class UStateCallbacks : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnStateEntered OnStateEntered;
	UPROPERTY(BlueprintAssignable)
	FOnStateExited OnStateExited;
};

class ABowlingPlayerState;
/**
 * 
 */
UCLASS()
class BOWLING_API ABowlingGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TMap<TEnumAsByte<EMatchState>, UStateCallbacks*> StateCallbacks;
	
	UFUNCTION(BlueprintCallable)
	UStateCallbacks* GetStateCallbacks(EMatchState state);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FinalFrame = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int NumPins = 10;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ActivePlayerIndex;

	UFUNCTION(BlueprintCallable)
	void CyclePlayer();

	UFUNCTION(BlueprintCallable)
	ABowlingPlayerState* GetActivePlayerState();

	static float GetFinalFrame(UWorld* WorldRef);

	static float GetNumPins(UWorld* WorldRef);
	void SetupStateCallbacks();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateAdded OnPlayerStateAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateRemoved OnPlayerStateRemoved;
};
