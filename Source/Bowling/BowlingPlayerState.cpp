// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPlayerState.h"

#include "BowlingGameModeBase.h"
#include "UI/BowlingScoreCard.h"

void ABowlingPlayerState::ReportPins(int numPins)
{
	if (Frames.Num() < (CurrentFrame + 1))
	{
		Frames.Add(FBowlingFrame());
	}

	FBowlingFrame& currentFrameData = Frames[CurrentFrame];

	if (CurrentBall == 0)
	{
		currentFrameData.ball1Pins = numPins;
	}
	if (CurrentBall == 1)
	{
		currentFrameData.ball2Pins = numPins;
	}
	if (CurrentBall == 2)
	{
		currentFrameData.ball3Pins = numPins;
	}

	CurrentBall++;

	int maxNumPins = ABowlingGameModeBase::GetNumPins(GetWorld());
	if ((CurrentFrame + 1) == ABowlingGameModeBase::GetFinalFrame(GetWorld()))
	{
		if ((currentFrameData.ball1Pins == maxNumPins || currentFrameData.ball2Pins == maxNumPins))
		{
			if(CurrentBall == 3)
			{
				CurrentBall = 0;
				CurrentFrame++;
			}
		}
		else if (CurrentBall == 2)
		{
			CurrentBall = 0;
			CurrentFrame++;
		}
	}
	else if (CurrentBall == 2 || currentFrameData.ball1Pins == maxNumPins)
	{
		CurrentBall = 0;
		CurrentFrame++;
	}

	Cast<ABowlerPlayerController>(GetPlayerController())->ScoreCardInstance->SyncWithGameState();
}

void ABowlingPlayerState::TestPins()
{
	static int lastValue = 0;
	static int lastFrame = CurrentFrame;
	int nextValue = FMath::RandRange(lastValue, 10);
	ReportPins(nextValue);
	if(lastFrame != CurrentFrame)
	{
		lastValue = 0;
	}
	else
	{
		lastValue = nextValue;
	}
}
