// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPlayerController.h"

void ABowlerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APlayerController::SetupInputComponent();
	check(InputComponent);
	ABowlerPawn* ControlledBowler = Cast<ABowlerPawn>(GetPawn());
	check(ControlledBowler);
		
	InputComponent->BindAxis("XMovement", ControlledBowler, &ABowlerPawn::MoveX);
}
