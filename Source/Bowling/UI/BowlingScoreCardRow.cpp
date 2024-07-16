// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCardRow.h"

#include "BowlingScoreCardEntry.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/WrapBox.h"

void UBowlingScoreCardRow::SyncWithPlayerState(TObjectPtr<ABowlingPlayerState> playerState)
{
	if (Entries.Num() == 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			auto entryWidget = CreateWidget<UBowlingScoreCardEntry>(GetOwningPlayer(), EntryTemplate);
			EntryContainer->AddChild(entryWidget);
			Entries.Add(entryWidget);
		}
	}
	for (int i = 0; i < playerState->Frames.Num(); ++i)
	{
		Entries[i]->SetScore(10);
	}
}
