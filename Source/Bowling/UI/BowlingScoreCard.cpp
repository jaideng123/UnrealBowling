// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCard.h"

#include "BowlingScoreCardRow.h"
#include "Bowling/BowlingGameStateBase.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UBowlingScoreCard::SyncWithGameState()
{
	ABowlingGameStateBase* gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	for (auto playerState : gameState->PlayerArray)
	{
		auto bowlingPlayerState = Cast<ABowlingPlayerState>(playerState);
		if (!Rows.Contains(playerState->GetPlayerId()))
		{
			auto widget = CreateWidget<UBowlingScoreCardRow>(GetOwningPlayer(), RowTemplate);
			RowContainer->AddChild(widget);
			Rows.Add(playerState->GetPlayerId(), widget);
		}
		UBowlingScoreCardRow* playerRow = Rows[playerState->GetPlayerId()];
		playerRow->SyncWithPlayerState(bowlingPlayerState);
	}
}
