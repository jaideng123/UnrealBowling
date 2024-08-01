// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingTitleScreen.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UBowlingTitleScreen::OnStartGame(int numPlayers)
{
	UGameplayStatics::OpenLevel(GetWorld(), GameplayMapUrl);
}

void UBowlingTitleScreen::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
