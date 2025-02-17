// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameStateBase.h"

#include "BowlingPlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABowlingGameStateBase::CyclePlayer()
{
	ActivePlayerIndex = FMath::Wrap(ActivePlayerIndex + 1,0,PlayerArray.Num()-1);
}

ABowlingPlayerState* ABowlingGameStateBase::GetActivePlayerState()
{
	return Cast<ABowlingPlayerState>(PlayerArray[ActivePlayerIndex]);
}

float ABowlingGameStateBase::GetFinalFrame(UWorld* WorldRef)
{
	ABowlingGameStateBase* bowlingGameMode = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(WorldRef));
	return bowlingGameMode->FinalFrame;
}

float ABowlingGameStateBase::GetNumPins(UWorld* WorldRef)
{
	ABowlingGameStateBase* bowlingGameMode = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(WorldRef));
	return bowlingGameMode->NumPins;
}