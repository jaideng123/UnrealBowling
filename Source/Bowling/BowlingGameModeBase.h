// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BowlerPawn.h"
#include "BowlerPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "BowlingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API ABowlingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void StartPlay() override;
public:
	// TODO: hook this up
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FinalFrame = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int NumPins = 10;

	UFUNCTION(BlueprintCallable)
	bool HasRoundEnded();

	UFUNCTION(BlueprintCallable)
	bool HasGameEnded();

	UFUNCTION(BlueprintCallable)
	bool ShouldResetPinsEarly();

	static float GetFinalFrame(UWorld* worldRef);

	static float GetNumPins(UWorld* worldRef);
};
