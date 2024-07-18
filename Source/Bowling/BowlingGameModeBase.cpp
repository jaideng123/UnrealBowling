// Copyright Epic Games, Inc. All Rights Reserved.


#include "BowlingGameModeBase.h"

#include "BowlingGameStateBase.h"
#include "BowlingPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABowlingGameModeBase::StartPlay()
{
	Super::StartPlay();
}

bool ABowlingGameModeBase::HasRoundEnded()
{
	ABowlingGameStateBase* gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);
	ABowlingPlayerState* activePlayerState = gameState->GetActivePlayerState();
	check(activePlayerState)

	return activePlayerState->CurrentBall == 0;
}

bool ABowlingGameModeBase::HasGameEnded()
{
	ABowlingGameStateBase* gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);

	// Check if any players have yet to complete their last frame
	for (auto playerState : gameState->PlayerArray)
	{
		ABowlingPlayerState* bowlingPlayerState = Cast<ABowlingPlayerState>(playerState);
		if (bowlingPlayerState->CurrentFrame < FinalFrame)
		{
			return false;
		}
	}

	return true;
}

bool ABowlingGameModeBase::ShouldResetPinsEarly()
{
	ABowlingGameStateBase* gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);
	ABowlingPlayerState* activePlayerState = gameState->GetActivePlayerState();
	check(activePlayerState)
	
	if((activePlayerState->CurrentFrame + 1) == FinalFrame)
	{
		FBowlingFrame& currentFrame = activePlayerState->Frames.Last();
		if(currentFrame.ball1Pins == NumPins && activePlayerState->CurrentBall == 1)
		{
			return true;
		}
	
		if(currentFrame.ball2Pins == NumPins && activePlayerState->CurrentBall == 2)
		{
			return true;
		}
	}

	return false;
}

float ABowlingGameModeBase::GetFinalFrame(UWorld* worldRef)
{
	ABowlingGameModeBase* bowlingGameMode = Cast<ABowlingGameModeBase>(UGameplayStatics::GetGameMode(worldRef));
	return bowlingGameMode->FinalFrame;
}

float ABowlingGameModeBase::GetNumPins(UWorld* worldRef)
{
	ABowlingGameModeBase* bowlingGameMode = Cast<ABowlingGameModeBase>(UGameplayStatics::GetGameMode(worldRef));
	return bowlingGameMode->NumPins;
}
