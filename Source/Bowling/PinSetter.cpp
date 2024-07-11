// Fill out your copyright notice in the Description page of Project Settings.


#include "PinSetter.h"

#include "BowlingGameModeBase.h"
#include "BowlingGameStateBase.h"
#include "BowlingPlayerState.h"
#include "Pin.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APinSetter::APinSetter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APinSetter::SpawnPins()
{
	for (auto spawnPoint : PinSpawnPoints)
	{
		APin* SpawnedPin = GetWorld()->SpawnActor<APin>(PinType, spawnPoint->GetActorLocation(),
		                                                spawnPoint->GetActorRotation());
		SpawnedPin->OriginalSpawn = spawnPoint;
		SpawnedPins.Add(SpawnedPin);
	}
}

void APinSetter::SweepPins_Implementation()
{
}

void APinSetter::ResetPins(bool reportScore)
{
	if(reportScore)
	{
		ReportPins();
	}
	for (const auto spawnedPin : SpawnedPins)
	{
		spawnedPin->ResetToSpawn();
		spawnedPin->TouchedByBall = false;
	}
}

void APinSetter::RaiseStandingPins()
{
	ReportPins();
	for (const auto spawnedPin : SpawnedPins)
	{
		if (!spawnedPin->IsToppled())
		{
			spawnedPin->RaisePin(GetActorLocation().Y);
			spawnedPin->TouchedByBall = false;
			RaisedPins.Add(spawnedPin);
		}
	}
}

void APinSetter::LowerStandingPins()
{
	for (const auto spawnedPin : RaisedPins)
	{
		spawnedPin->LowerPin();
	}
	RaisedPins.Empty();
}

void APinSetter::ReportPins()
{
	int pinKnockedCount = 0;
	for (auto pin : SpawnedPins)
	{
		if(pin->IsToppled())
		{
			pinKnockedCount++;
		}
	}

	TObjectPtr<ABowlingGameStateBase> gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);
	gameState->GetActivePlayerState()->ReportPins(pinKnockedCount);
}

// Called when the game starts or when spawned
void APinSetter::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<ABowlingGameModeBase> gameMode = Cast<ABowlingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);
	gameMode->NumPins = PinSpawnPoints.Num();
}

// Called every frame
void APinSetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
