// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BowlingGameStateBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAdded, ABowlingPlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateRemoved, int32, PlayerId);

class ABowlingPlayerState;
/**
 * 
 */
UCLASS()
class BOWLING_API ABowlingGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateAdded OnPlayerStateAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateRemoved OnPlayerStateRemoved;
};
