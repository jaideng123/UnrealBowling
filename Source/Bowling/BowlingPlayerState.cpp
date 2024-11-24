// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPlayerState.h"

#include "BowlingGameModeBase.h"
#include "Logging/StructuredLog.h"
#include "UI/BowlingScoreCard.h"

void ABowlingPlayerState::RecalculateScore(const TArray<FBowlingFrame>& FramesToScore) const
{
	int accumulatedScore = 0;
	for (int i = 0; i < FramesToScore.Num(); ++i)
	{
		int frameScore = 0;

		const auto& currentFrame = FramesToScore[i];

		if (currentFrame.ball1Pins == -1)
		{
			continue;
		}

		// Handle Final Frame
		if (i == ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1)
		{
			frameScore += currentFrame.ball1Pins == -1 ? 0 : currentFrame.ball1Pins;
			if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				frameScore += currentFrame.ball2Pins == -1 ? 0 : currentFrame.ball2Pins;
				frameScore += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;
			}

			frameScore += currentFrame.ball2Pins == -1 ? 0 : currentFrame.ball2Pins;
			if (currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				frameScore += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;
			}

			frameScore += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;

			accumulatedScore += frameScore;
			currentFrame.score = accumulatedScore;
			continue;
		}

		frameScore += currentFrame.ball1Pins == -1 ? 0 : currentFrame.ball1Pins;
		frameScore += currentFrame.ball2Pins == -1 ? 0 : currentFrame.ball2Pins;

		if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if (FramesToScore.Num() > i + 1)
			{
				const auto& nextFrame = FramesToScore[i + 1];
				if (i + 1 == (ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1))
				{
					frameScore += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					frameScore += nextFrame.ball2Pins == -1 ? 0 : nextFrame.ball2Pins;
					if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
					{
						frameScore += nextFrame.ball2Pins == -1 ? 0 : nextFrame.ball2Pins;
					}
					else
					{
						frameScore += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
					}
				}
				else if (nextFrame.ball1Pins != ABowlingGameModeBase::GetNumPins(GetWorld()))
				{
					frameScore += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					frameScore += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
				}
				else
				{
					frameScore += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					if (FramesToScore.Num() > i + 2)
					{
						const auto& nextNextFrame = FramesToScore[i + 2];
						frameScore += nextNextFrame.ball1Pins == -1 ? 0 : nextNextFrame.ball1Pins;
					}
				}
			}
		}
		else if (currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if (FramesToScore.Num() < i + 1)
			{
				const auto& nextFrame = FramesToScore[i + 1];
				frameScore += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
			}
		}

		accumulatedScore += frameScore;
		currentFrame.score = accumulatedScore;
	}
}

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

	RecalculateScore(Frames);

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
	int lastValue = Frames.Num() == 0 ||
	                Frames.Last().ball1Pins == -1 ||
	                Frames.Last().ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld())
		                ? 0
		                : Frames.Last().ball1Pins;
	static int lastFrame = CurrentFrame;
	int nextValue = FMath::RandRange(lastValue, 10);
	UE_LOGFMT(LogTemp, Error, "{0}", nextValue);
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
