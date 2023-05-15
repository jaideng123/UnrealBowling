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

	InputComponent->BindAxis("YBallMovement", ControlledBowler, &ABowlerPawn::MoveBallY);
	InputComponent->BindAxis("XBallMovement", ControlledBowler, &ABowlerPawn::MoveBallX);

	InputComponent->BindTouch(IE_Pressed, this, &ABowlerPlayerController::HandleTouchPress);
	InputComponent->BindTouch(IE_Released, this, &ABowlerPlayerController::HandleTouchRelease);
	InputComponent->BindTouch(IE_Repeat, this, &ABowlerPlayerController::HandleTouchHeld);
}

void ABowlerPlayerController::HandleTouchPress(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Touch Press: %d Location: %s"), touchIndex, *location.ToString()));
	ControlledBowler->GripBall();
}

void ABowlerPlayerController::HandleTouchRelease(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Touch Release: %d Location: %s"), touchIndex, *location.ToString()));
	ControlledBowler->ReleaseBall();
}

void ABowlerPlayerController::HandleTouchHeld(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	static UE::Math::TVector<double> lastPosition = location;
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, FString::Printf(TEXT("Touch Held: %d Location: %s"), touchIndex, *location.ToString()));
	int screenSizeX;
	int screenSizeY;
	GetViewportSize(screenSizeX,screenSizeY);
	float xPercentageMoved = (lastPosition - location).X / screenSizeX;
	float yPercentageMoved = (lastPosition - location).Y / screenSizeY;
	float totalY = ControlledBowler->MaxArmAngle - ControlledBowler->MinArmAngle;
	float totalX = ControlledBowler->MaxBallSpin;
	
	ControlledBowler->MoveBallX(xPercentageMoved * 1 * totalX * -1);
	ControlledBowler->MoveBallY(yPercentageMoved * 3 * totalY);
	lastPosition = location;
}
