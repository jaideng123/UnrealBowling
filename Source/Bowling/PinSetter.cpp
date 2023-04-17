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
	for(auto spawnPoint : PinSpawnPoints)
	{
		APin* SpawnedPin = GetWorld()->SpawnActor<APin>(PinType, spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation());
		SpawnedPin->OriginalSpawn = spawnPoint;
		SpawnedPins.Add(SpawnedPin);
	}
}

void APinSetter::SweepPins_Implementation()
{
}

void APinSetter::ResetPins()
{
	for(const auto spawnedPin : SpawnedPins)
	{
		spawnedPin->SetActorLocationAndRotation(spawnedPin->OriginalSpawn->GetActorLocation(), PinType.GetDefaultObject()->GetActorRotation());
	}
}

void APinSetter::RaiseStandingPins()
{
	for(const auto spawnedPin : SpawnedPins)
	{
		// TODO: check dist to spawn / not in drop zone
		if(FVector::DotProduct(spawnedPin->GetActorUpVector(), FVector::UpVector) >= .95)
		{
			spawnedPin->RaisePin(GetActorLocation().Y);
		}
	}
}

void APinSetter::LowerStandingPins()
{
	for(const auto spawnedPin : SpawnedPins)
	{
		// TODO: this is a bad check, should keep list of raised pins
		if(FVector::DotProduct(spawnedPin->GetActorUpVector(), FVector::UpVector) >= .95)
		{
			spawnedPin->LowerPin();
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
