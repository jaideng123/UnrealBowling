// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingEndUI.h"

#include "Kismet/GameplayStatics.h"

void UBowlingEndUI::RestartGame()
{
	// TODO: reset all state instead of reopening the level
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UBowlingEndUI::ReturnToTitleScreen()
{
	UGameplayStatics::OpenLevel(GetWorld(), TitleMapUrl);
}
