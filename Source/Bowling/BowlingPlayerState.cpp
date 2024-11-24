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

	FBowlingFrame& currentFrame = Frames[CurrentFrame];

	if (CurrentBall == 0)
	{
		currentFrame.ball1Pins = numPins;
	}
	if (CurrentBall == 1)
	{
		if (CurrentFrame == ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1)
		{
			if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				currentFrame.ball2Pins = numPins;
			}
			else
			{
				currentFrame.ball2Pins = numPins - currentFrame.ball1Pins;
			}
		}
		else
		{
			currentFrame.ball2Pins = numPins - currentFrame.ball1Pins;
		}
	}
	if (CurrentBall == 2)
	{
		// We should only be throwing a 3rd ball in the final frame
		check(CurrentFrame == ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1);

		// Check if we got a strike or a spare on ball 2
		if (currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()) ||
			currentFrame.ball2Pins + currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			currentFrame.ball3Pins = numPins;
		}
		else
		{
			currentFrame.ball3Pins = numPins - currentFrame.ball2Pins;
		}
	}

	//TODO: Calc score

	CurrentBall++;

	int maxNumPins = ABowlingGameModeBase::GetNumPins(GetWorld());
	if ((CurrentFrame + 1) == ABowlingGameModeBase::GetFinalFrame(GetWorld()))
	{
		// Allow a 3rd ball when a strike or spare occurs
		if (currentFrame.ball1Pins == maxNumPins || currentFrame.ball1Pins + currentFrame.ball2Pins == maxNumPins)
		{
			if (CurrentBall == 3)
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
	else if (CurrentBall == 2 || currentFrame.ball1Pins == maxNumPins)
	{
		CurrentBall = 0;
		CurrentFrame++;
	}

	OnScoreChangedDelegate.Broadcast(this);
}

void ABowlingPlayerState::TestPins()
{
	static int lastValue = 0;
	static int lastFrame = CurrentFrame;
	int nextValue = FMath::RandRange(lastValue, 10);
	ReportPins(nextValue);
	if (lastFrame != CurrentFrame)
	{
		lastValue = 0;
	}
	else
	{
		lastValue = nextValue;
	}
}
