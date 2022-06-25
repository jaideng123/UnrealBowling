// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPlayerController.h"

#include "Camera/CameraComponent.h"

void ABowlerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APlayerController::SetupInputComponent();
	check(InputComponent);
	ABowlerPawn* ControlledBowler = Cast<ABowlerPawn>(GetPawn());
	SetViewTarget(ControlledBowler);
	check(ControlledBowler != nullptr);
		
	InputComponent->BindAxis("XMovement", ControlledBowler, &ABowlerPawn::MoveX);
	
	InputComponent->BindAction("GripBall", IE_Pressed, ControlledBowler, &ABowlerPawn::GripBall);
	InputComponent->BindAction("GripBall", IE_Released, ControlledBowler, &ABowlerPawn::ReleaseBall);

	InputComponent->BindAxis("YBallMovement", ControlledBowler, &ABowlerPawn::MoveBallY);
	InputComponent->BindAxis("XBallMovement", ControlledBowler, &ABowlerPawn::MoveBallX);
	
}
