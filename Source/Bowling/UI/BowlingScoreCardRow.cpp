// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardRow.h"

#include "BowlingScoreCardEntry.h"
#include "Bowling/BowlingGameModeBase.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/WrapBox.h"

void UBowlingScoreCardRow::SyncWithPlayerState(TObjectPtr<ABowlingPlayerState> playerState)
{
	if (Entries.Num() == 0)
	{
		for (int i = 0; i < ABowlingGameModeBase::GetFinalFrame(GetWorld()); ++i)
		{
			auto entryWidget = CreateWidget<UBowlingScoreCardEntry>(GetOwningPlayer(), EntryTemplate);
			EntryContainer->AddChild(entryWidget);
			Entries.Add(entryWidget);
		}
	}
	int cumulativeScore = 0;
	for (int i = 0; i < playerState->Frames.Num(); ++i)
	{
		const auto& currentFrame = playerState->Frames[i];
		
		int score = 0;
		
		if(currentFrame.ball1Pins == -1)
		{
			score = -1;
		}
		else
		{
			score += currentFrame.ball1Pins == -1 ? 0 : currentFrame.ball1Pins;
			score += currentFrame.ball2Pins == -1 ? 0 : (currentFrame.ball2Pins - currentFrame.ball1Pins);
		}

		// check for strikes + spares
		if(currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if(playerState->Frames.Num() > i + 1)
			{
				const auto& nextFrame = playerState->Frames[i+1];
				if(nextFrame.ball1Pins != ABowlingGameModeBase::GetNumPins(GetWorld()))
				{
					score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					score += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
				}
				else
				{
					score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					if(playerState->Frames.Num() > i + 2)
					{
						const auto& nextNextFrame = playerState->Frames[i+2];
						score += nextNextFrame.ball1Pins == -1 ? 0 : nextNextFrame.ball1Pins;
					}

				}
			}
		}
		else if(currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if(playerState->Frames.Num() < i + 1)
			{
				const auto& nextFrame = playerState->Frames[i+1];
				score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
			}
		}


		auto& entry = Entries[i];
		if(score != -1)
		{
			cumulativeScore += score;
			entry->SetScore(cumulativeScore);
		}
		else
		{
			entry->SetScore(score);
		}
		entry->SetBall1(currentFrame.ball1Pins);
		entry->SetBall2(currentFrame.ball2Pins - currentFrame.ball1Pins);

	}
}

int UBowlingScoreCardRow::CalculateRawFrameScore(const FBowlingFrame& frame)
{
	if(frame.ball2Pins != -1)
	{
		return frame.ball2Pins;
	}
	else if(frame.ball1Pins != -1)
	{
		return frame.ball2Pins;
	}
	return 0;
	
}
