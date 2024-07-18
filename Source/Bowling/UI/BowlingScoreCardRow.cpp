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
