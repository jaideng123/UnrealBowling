// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPlayerController.h"

#include "Camera/CameraComponent.h"

void ABowlerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APlayerController::SetupInputComponent();
	check(InputComponent);
	ControlledBowler = Cast<ABowlerPawn>(GetPawn());
	SetViewTarget(ControlledBowler);
	check(ControlledBowler != nullptr);

	InputComponent->BindAxis("XMovement", ControlledBowler, &ABowlerPawn::MoveX);

	InputComponent->BindAction("GripBall", IE_Pressed, ControlledBowler, &ABowlerPawn::GripBall);
	InputComponent->BindAction("GripBall", IE_Released, ControlledBowler, &ABowlerPawn::ReleaseBall);

	InputComponent->BindAction("ToggleMovement", IE_Released, ControlledBowler, &ABowlerPawn::ToggleMovementMode);

	InputComponent->BindAxis("YBallMovement", ControlledBowler, &ABowlerPawn::MoveBallY);
	InputComponent->BindAxis("XBallMovement", ControlledBowler, &ABowlerPawn::MoveBallX);

	InputComponent->BindTouch(IE_Pressed, this, &ABowlerPlayerController::HandleTouchPress);
	InputComponent->BindTouch(IE_Released, this, &ABowlerPlayerController::HandleTouchRelease);
	InputComponent->BindTouch(IE_Repeat, this, &ABowlerPlayerController::HandleTouchHeld);
}

void ABowlerPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(CurrentContinuousMove != 0.0f)
	{
		ControlledBowler->MoveX(CurrentContinuousMove);
	}
}

void ABowlerPlayerController::HandleTouchPress(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Touch Press: %d Location: %s"), touchIndex, *location.ToString()));
	ControlledBowler->GripBall();
}

void ABowlerPlayerController::HandleTouchRelease(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,
	                                 FString::Printf(TEXT("Touch Release: %d Location: %s"), touchIndex, *location.ToString()));
	ControlledBowler->ReleaseBall();
	LastHoldPosition = NullPos;
}

void ABowlerPlayerController::HandleTouchHeld(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	if(LastHoldPosition == NullPos)
	{
		LastHoldPosition = location;
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, FString::Printf(TEXT("Touch Held: %d Location: %s"), touchIndex, *location.ToString()));
	int screenSizeX;
	int screenSizeY;
	GetViewportSize(screenSizeX, screenSizeY);
	float xPercentageMoved = (LastHoldPosition - location).X / screenSizeX;
	float yPercentageMoved = (LastHoldPosition - location).Y / screenSizeY;
	float totalY = ControlledBowler->MaxArmAngle - ControlledBowler->MinArmAngle;
	float totalX = ControlledBowler->MaxBallSpin / 2;

	ControlledBowler->MoveBallX(xPercentageMoved * 1 * totalX * -1);
	ControlledBowler->MoveBallY(yPercentageMoved * 3 * totalY);
	LastHoldPosition = location;
}

void ABowlerPlayerController::StartContinuousMove(float Direction)
{
	CurrentContinuousMove = Direction;
}

void ABowlerPlayerController::StopContinuousMove()
{
	CurrentContinuousMove = 0.0f;
}
