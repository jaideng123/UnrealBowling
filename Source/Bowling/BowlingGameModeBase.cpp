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
	// TODO: fixme
	return activePlayerState->CurrentBall == 0 && activePlayerState->Frames.Num() > 0 && activePlayerState->Frames.Last().ball1Pins == -1;
}

bool ABowlingGameModeBase::HasGameEnded()
{
	ABowlingGameStateBase* gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);

	// Check if any players have yet to complete their last frame
	for (auto playerState : gameState->PlayerArray)
	{
		ABowlingPlayerState* bowlingPlayerState = Cast<ABowlingPlayerState>(playerState);
		if(bowlingPlayerState->CurrentFrame < FinalFrame)
		{
			return false;
		}
	}
	
	return true;
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
