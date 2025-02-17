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
		if (bowlingPlayerState->CurrentFrame < ABowlingGameStateBase::GetFinalFrame(GetWorld()))
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
	
	if((activePlayerState->CurrentFrame + 1) == ABowlingGameStateBase::GetFinalFrame(GetWorld()))
	{
		FBowlingFrame& currentFrame = activePlayerState->Frames.Last();
		if(currentFrame.ball1Pins == ABowlingGameStateBase::GetNumPins(GetWorld()) && activePlayerState->CurrentBall == 1)
		{
			return true;
		}
	
		if(currentFrame.ball1Pins + currentFrame.ball2Pins == ABowlingGameStateBase::GetNumPins(GetWorld()) && activePlayerState->CurrentBall == 2)
		{
			return true;
		}
	}

	return false;
}


