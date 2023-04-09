// Fill out your copyright notice in the Description page of Project Settings.


#include "PinSetter.h"

#include "Pin.h"

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
		APin* SpawnedPin = GetWorld()->SpawnActor<APin>(PinType,spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation());
		SpawnedPin->OriginalSpawn = spawnPoint;
		SpawnedPins.Add(SpawnedPin);
	}
}

void APinSetter::SweepPins()
{
	for (auto SpawnedPin : SpawnedPins)
	{
		if(FVector::DotProduct(SpawnedPin->GetActorUpVector(),FVector::UpVector) < .95)
		{
			
		}
	}
}

void APinSetter::RaiseStandingPins()
{
	for (auto SpawnedPin : SpawnedPins)
	{
		// TODO: check dist to spawn / not in drop zone
		if(FVector::DotProduct(SpawnedPin->GetActorUpVector(),FVector::UpVector) >= .95)
		{
			SpawnedPin->RaisePin(GetActorLocation().Y);
		}
	}
}

void APinSetter::LowerStandingPins()
{
	for (auto SpawnedPin : SpawnedPins)
	{
		// TODO: this is a bad check, should keep list of raised pins
		if(FVector::DotProduct(SpawnedPin->GetActorUpVector(),FVector::UpVector) >= .95)
		{
			SpawnedPin->LowerPin();
		}
	}
}

// Called when the game starts or when spawned
void APinSetter::BeginPlay()
{
	Super::BeginPlay();

	SpawnPins();
}

// Called every frame
void APinSetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
