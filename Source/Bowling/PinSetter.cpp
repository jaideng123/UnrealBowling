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

	bReplicates = true;
}

void APinSetter::SpawnPins()
{
	if(HasAuthority())
	{
		for(auto spawnPoint : PinSpawnPoints)
		{
			APin* SpawnedPin = GetWorld()->SpawnActor<APin>(PinType, spawnPoint->GetActorLocation(),
			                                                spawnPoint->GetActorRotation());
			SpawnedPin->OriginalSpawn = spawnPoint;
			SpawnedPins.Add(SpawnedPin);
		}
	}
}

void APinSetter::SweepPins_Implementation()
{
}

void APinSetter::ResetPins()
{
	if(!HasAuthority())
	{
		return;
	}
	for(const auto spawnedPin : SpawnedPins)
	{
		spawnedPin->ResetToSpawn();
		spawnedPin->TouchedByBall = false;
	}
}

void APinSetter::RaiseStandingPins()
{
	if(!HasAuthority())
	{
		return;
	}
	for(const auto spawnedPin : SpawnedPins)
	{
		if(!spawnedPin->IsToppled())
		{
			spawnedPin->RaisePin(GetActorLocation().Y);
			spawnedPin->TouchedByBall = false;
			RaisedPins.Add(spawnedPin);
		}
	}
}

void APinSetter::LowerStandingPins()
{
	if(!HasAuthority())
	{
		return;
	}
	for(const auto spawnedPin : RaisedPins)
	{
		spawnedPin->LowerPin();
	}
	RaisedPins.Empty();
}

void APinSetter::ReportPins()
{
	if(!HasAuthority())
	{
		return;
	}
	int pinKnockedCount = 0;
	for(auto pin : SpawnedPins)
	{
		if(pin->IsToppled())
		{
			pinKnockedCount++;
		}
	}

	TObjectPtr<ABowlingGameStateBase> gameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	check(gameState);

	bool alwaysSpare = false;
	bool alwaysStrike = false;
	if((gameState->GetActivePlayerState()->CurrentBall >= 1 && alwaysSpare) || alwaysStrike)
	{
		gameState->GetActivePlayerState()->ReportPins(10);
	}
	else
	{
		gameState->GetActivePlayerState()->ReportPins(pinKnockedCount);
	}
}

void APinSetter::UpdateNumPins()
{
	if(!HasAuthority())
	{
		return;
	}
	if(const TObjectPtr<ABowlingGameStateBase> GameState = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameState->NumPins = PinSpawnPoints.Num();
	}
}

// Called when the game starts or when spawned
void APinSetter::BeginPlay()
{
	Super::BeginPlay();
	UpdateNumPins();
}

// Called every frame
void APinSetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
