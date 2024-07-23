// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardRow.h"

#include "BowlingScoreCardEntry.h"
#include "BowlingScoreCardFinalEntry.h"
#include "Bowling/BowlingGameModeBase.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/WrapBox.h"

void UBowlingScoreCardRow::SyncWithPlayerState(TObjectPtr<ABowlingPlayerState> playerState)
{
	if (Entries.Num() == 0)
	{
		for (int i = 0; i < ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1; ++i)
		{
			auto entryWidget = CreateWidget<UBowlingScoreCardEntry>(GetOwningPlayer(), EntryTemplate);
			EntryContainer->AddChild(entryWidget);
			Entries.Add(entryWidget);
		}
		FinalEntry = CreateWidget<UBowlingScoreCardFinalEntry>(GetOwningPlayer(), FinalEntryTemplate);
		EntryContainer->AddChild(FinalEntry);
	}
	int cumulativeScore = 0;
	for (int i = 0; i < playerState->Frames.Num(); ++i)
	{
		const auto& currentFrame = playerState->Frames[i];

		int score = 0;

		// Handle Final Frame
		if (i == ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1)
		{
			if (currentFrame.ball1Pins == -1)
			{
				FinalEntry->SetScore(-1);
				continue;
			}
			score += currentFrame.ball1Pins == -1 ? 0 : currentFrame.ball1Pins;
			if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				score += currentFrame.ball2Pins == -1 ? 0 : currentFrame.ball2Pins;
				score += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;
			}

			score += currentFrame.ball2Pins == -1 ? 0 : currentFrame.ball2Pins;
			if (currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				score += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;
			}

			score += currentFrame.ball3Pins == -1 ? 0 : currentFrame.ball3Pins;

			FinalEntry->SetScore(score);
			FinalEntry->SetBall1(currentFrame.ball1Pins);
			if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
			{
				FinalEntry->SetBall2(currentFrame.ball2Pins, false);
			}
			else
			{
				FinalEntry->SetBall2(currentFrame.ball2Pins - currentFrame.ball1Pins, true);
			}
			FinalEntry->SetBall3(currentFrame.ball3Pins,
			                     currentFrame.ball2Pins != ABowlingGameModeBase::GetNumPins(GetWorld()));
			continue;
		}

		if (currentFrame.ball1Pins == -1)
		{
			score = -1;
		}
		else
		{
			score += currentFrame.ball1Pins == -1 ? 0 : currentFrame.ball1Pins;
			score += currentFrame.ball2Pins == -1 ? 0 : (currentFrame.ball2Pins - currentFrame.ball1Pins);
		}

		// check for strikes + spares
		if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if (playerState->Frames.Num() > i + 1)
			{
				// TODO: handle final frame
				const auto& nextFrame = playerState->Frames[i + 1];
				if (i + 1 == (ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1))
				{
					score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					score += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
					if (currentFrame.ball1Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
					{
						score += nextFrame.ball2Pins == -1 ? 0 : nextFrame.ball2Pins;
					}
					else
					{
						score += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
					}
				}
				else if (nextFrame.ball1Pins != ABowlingGameModeBase::GetNumPins(GetWorld()))
				{
					score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					score += nextFrame.ball2Pins == -1 ? 0 : (nextFrame.ball2Pins - nextFrame.ball1Pins);
				}
				else
				{
					score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
					if (playerState->Frames.Num() > i + 2)
					{
						const auto& nextNextFrame = playerState->Frames[i + 2];
						score += nextNextFrame.ball1Pins == -1 ? 0 : nextNextFrame.ball1Pins;
					}
				}
			}
		}
		else if (currentFrame.ball2Pins == ABowlingGameModeBase::GetNumPins(GetWorld()))
		{
			if (playerState->Frames.Num() < i + 1)
			{
				const auto& nextFrame = playerState->Frames[i + 1];
				score += nextFrame.ball1Pins == -1 ? 0 : nextFrame.ball1Pins;
			}
		}


		auto& entry = Entries[i];
		if (score != -1)
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
	if (frame.ball2Pins != -1)
	{
		return frame.ball2Pins;
	}
	else if (frame.ball1Pins != -1)
	{
		return frame.ball2Pins;
	}
	return 0;
}
