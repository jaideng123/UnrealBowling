// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreCard.h"

#include "BowlingScoreCardRow.h"
#include "Bowling/BowlingGameStateBase.h"
#include "Bowling/BowlingPlayerState.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UBowlingScoreCard::NativeConstruct()
{
	Super::NativeConstruct();
	for (TObjectPtr<APlayerState> PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		ABowlingPlayerState* BowlingPlayerState = Cast<ABowlingPlayerState>(PlayerState);
		if (BowlingPlayerState)
		{
			BowlingPlayerState->OnScoreChangedDelegate.AddUniqueDynamic(this, &UBowlingScoreCard::SyncWithPlayerState);
			SyncWithPlayerState(BowlingPlayerState);
		}
	}
}

void UBowlingScoreCard::SyncWithPlayerState(ABowlingPlayerState* PlayerState)
{
	if (!Rows.Contains(PlayerState->GetPlayerId()))
	{
		auto widget = CreateWidget<UBowlingScoreCardRow>(GetOwningPlayer(), RowTemplate);
		RowContainer->AddChild(widget);
		Rows.Add(PlayerState->GetPlayerId(), widget);
	}
	UBowlingScoreCardRow* playerRow = Rows[PlayerState->GetPlayerId()];
	playerRow->SyncWithPlayerState(PlayerState);
}
