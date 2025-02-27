﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Pin.h"


// Sets default values
APin::APin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APin::BeginPlay()
{
	Super::BeginPlay();
	PrimitiveComponent = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
}

// Called every frame
void APin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(ShowCenterOfMass)
	{
		DrawDebugCircle(GetWorld(), PrimitiveComponent->GetCenterOfMass(), 4.0, 10, FColor::Magenta, false, 0, SDPG_Foreground, 1);
	}
}

void APin::ResetToSpawn()
{
	PrimitiveComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
	PrimitiveComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
	SetActorLocationAndRotation(OriginalSpawn->GetActorLocation(),
	                            Cast<AActor>(StaticClass()->GetDefaultObject())->GetActorRotation());
}

bool APin::IsToppled() const
{
	auto DistanceFromSpawn = (GetActorLocation() - OriginalSpawn->GetActorLocation());
	DistanceFromSpawn.Z = 0;
	return FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < .95 ||
		DistanceFromSpawn.Length() > 10;
}

void APin::RaisePin_Implementation(double X)
{
}

void APin::LowerPin_Implementation()
{
}
