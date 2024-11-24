// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardRow.h"

#include "BowlingScoreCardEntry.h"
#include "BowlingScoreCardFinalEntry.h"
#include "Bowling/BowlingGameModeBase.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Logging/StructuredLog.h"

void UBowlingScoreCardRow::SyncWithPlayerState(TObjectPtr<ABowlingPlayerState> playerState)
{
	if (Entries.Num() == 0)
	{
		for (int i = 0; i < ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1; ++i)
		{
			auto entryWidget = CreateWidget<UBowlingScoreCardEntry>(GetOwningPlayer(), EntryTemplate);
			entryWidget->SetFrameNumber(i + 1);
			EntryContainer->AddChild(entryWidget);
			Entries.Add(entryWidget);
		}
		FinalEntry = CreateWidget<UBowlingScoreCardFinalEntry>(GetOwningPlayer(), FinalEntryTemplate);
		EntryContainer->AddChild(FinalEntry);
	}
	if (playerState->CurrentFrame < ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1)
	{
		ScrollBox->ScrollWidgetIntoView(Entries[playerState->CurrentFrame]);
	}
	else
	{
		ScrollBox->ScrollWidgetIntoView(FinalEntry);
	}
	for (int i = 0; i < playerState->Frames.Num(); ++i)
	{
		const auto& currentFrame = playerState->Frames[i];
		// Handle Final Frame
		if (i == ABowlingGameModeBase::GetFinalFrame(GetWorld()) - 1)
		{
			FinalEntry->SetScore(currentFrame.score);
			FinalEntry->SetBall1(currentFrame.ball1Pins);
			FinalEntry->SetBall2(currentFrame.ball2Pins,
			                     currentFrame.ball1Pins != ABowlingGameModeBase::GetNumPins(GetWorld()));
			FinalEntry->SetBall3(currentFrame.ball3Pins,
			                     currentFrame.ball2Pins != ABowlingGameModeBase::GetNumPins(GetWorld()));
			continue;
		}

		if (i > Entries.Num() - 1)
		{
			UE_LOGFMT(LogTemp, Error, "Exceeded Entry Count!");
			break;
		}

		const auto& currentEntry = Entries[i];

		currentEntry->SetScore(currentFrame.score);
		currentEntry->SetBall1(currentFrame.ball1Pins);
		currentEntry->SetBall2(currentFrame.ball2Pins);
	}
}
