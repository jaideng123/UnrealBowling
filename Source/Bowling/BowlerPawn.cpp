// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPawn.h"

// Sets default values
ABowlerPawn::ABowlerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABowlerPawn::BeginPlay()
{
	Super::BeginPlay();
	CurrentBall = GetWorld()->SpawnActor<ABallBase>(BallClass, GetActorLocation(), GetActorRotation());
}

// Called every frame
void ABowlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABowlerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
