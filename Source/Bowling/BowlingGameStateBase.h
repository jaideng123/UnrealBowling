// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BowlingGameStateBase.generated.h"

class ABowlingPlayerState;
/**
 * 
 */
UCLASS()
class BOWLING_API ABowlingGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ActivePlayerIndex;

	UFUNCTION(BlueprintCallable)
	void CyclePlayer();

	UFUNCTION(BlueprintCallable)
	ABowlingPlayerState* GetActivePlayerState();
};
