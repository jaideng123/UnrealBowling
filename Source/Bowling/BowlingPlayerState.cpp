// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPlayerState.h"

#include "BowlingGameModeBase.h"

void ABowlingPlayerState::ReportPins(int numPins)
{
	if(Frames.Num() < (CurrentFrame + 1))
	{
		Frames.Add(FBowlingFrame());
	}

	FBowlingFrame& currentFrameData = Frames[CurrentFrame];

	if(CurrentBall == 0)
	{
		currentFrameData.ball1Pins = numPins;
	}
	if(CurrentBall == 1)
	{
		currentFrameData.ball2Pins = numPins;
	}

	if(CurrentBall == 2)
	{
		currentFrameData.ball3Pins = numPins;
	}

	CurrentBall++;

	if (CurrentFrame == ABowlingGameModeBase::GetFinalFrame(GetWorld()))
	{
		if ((currentFrameData.ball1Pins == 10 || currentFrameData.ball2Pins == 10) && CurrentBall == 3)
		{
			CurrentBall = 0;
			CurrentFrame++;
		}
		else if(CurrentBall == 2)
		{
			CurrentBall = 0;
			CurrentFrame++;
		}
	}
	else if (CurrentBall == 2 || currentFrameData.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
	{
		CurrentBall = 0;
		CurrentFrame++;
	}
	
}

void ABowlingPlayerState::TestPins()
{
	ReportPins(FMath::RandRange(0,10));
}
