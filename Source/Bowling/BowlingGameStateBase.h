// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "BowlingGameStateBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAdded, ABowlingPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoved, int32, PlayerId);

UENUM(BlueprintType)
enum EMatchState
{
	UNSET,
	START,
	PRE_BOWL,
	BOWL_SETUP,
	THROWING,
	THROWN,
	OUT_OF_BOUNDS,
	BOWL_COMPLETE,
	CLEARING_PINS,
	CYCLE_BOWLER,
	GAME_END,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateExited);

UCLASS(BlueprintType)
class UStateCallbacks : public UObject
{
	GENERATED_BODY()
public:
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
	
	UFUNCTION(BlueprintCallable, Category = "BowlingUtils", BlueprintPure, meta=( WorldContext = "Context" ))
	static UStateCallbacks* GetCallbacksForBowlingState(UObject* Context, TEnumAsByte<EMatchState> State);

	UFUNCTION(BlueprintCallable, Category = "BowlingUtils", BlueprintPure, meta=( WorldContext = "Context" ))
	static ABowlingGameStateBase* GetBowlingGameState(UObject* Context);

	UFUNCTION()
	void OnRepMatchState(TEnumAsByte<EMatchState> PreviousState);

	UFUNCTION(BlueprintCallable)
	void SetMatchState(EMatchState NewMatchState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMatchState GetMatchState() const;
	
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

	UFUNCTION(BlueprintCallable, Category = "BowlingUtils")
	static float GetNumPins(UWorld* WorldRef);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateAdded OnPlayerStateAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateRemoved OnPlayerStateRemoved;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRepMatchState)
	TEnumAsByte<EMatchState> CurrentMatchState;
};

inline EMatchState ABowlingGameStateBase::GetMatchState() const
{
	return CurrentMatchState;
}

inline void ABowlingGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABowlingGameStateBase, CurrentMatchState);
}
