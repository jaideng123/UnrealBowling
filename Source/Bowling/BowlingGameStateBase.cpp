// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameStateBase.h"

#include "BowlingPlayerState.h"

void ABowlingGameStateBase::CyclePlayer()
{
	ActivePlayerIndex = FMath::Wrap(ActivePlayerIndex + 1,0,PlayerArray.Num()-1);
}

ABowlingPlayerState* ABowlingGameStateBase::GetActivePlayerState()
{
	return Cast<ABowlingPlayerState>(PlayerArray[ActivePlayerIndex]);
}
