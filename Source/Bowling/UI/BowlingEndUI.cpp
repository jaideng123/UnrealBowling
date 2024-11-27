// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingEndUI.h"

#include "Bowling/BowlingPlayerState.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

class ABowlingPlayerState;

void UBowlingEndUI::RestartGame()
{
	// TODO: reset all state instead of reopening the level
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UBowlingEndUI::ReturnToTitleScreen()
{
	UGameplayStatics::OpenLevel(GetWorld(), TitleMapUrl);
}

void UBowlingEndUI::NativeConstruct()
{
	Super::NativeConstruct();
	for (TObjectPtr<APlayerState> PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		ABowlingPlayerState* BowlingPlayerState = Cast<ABowlingPlayerState>(PlayerState);
		if (BowlingPlayerState)
		{
			BowlingPlayerState->OnScoreChangedDelegate.AddUniqueDynamic(this, &UBowlingEndUI::SyncWithPlayerState);
			SyncWithPlayerState(BowlingPlayerState);
		}
	}
}

void UBowlingEndUI::SyncWithPlayerState(ABowlingPlayerState* playerState)
{
	if (playerState->Frames.Num() > 0)
	{
		WinningScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), playerState->Frames.Last().score)));
	}
}
