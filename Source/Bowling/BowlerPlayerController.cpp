// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPlayerController.h"

#include "BowlingGameStateBase.h"
#include "BowlingGameUserSettings.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BowlingScoreCard.h"

void ABowlerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APlayerController::SetupInputComponent();
	check(InputComponent);
	ControlledBowler = Cast<ABowlerPawn>(GetPawn());
	if(IsLocalController())
	{
		InputComponent->BindAxis("XMovement", this, &ABowlerPlayerController::AttemptMoveX);

		// InputComponent->BindAction("GripBall", IE_Pressed, ControlledBowler, &ABowlerPawn::GripBall);
		// InputComponent->BindAction("GripBall", IE_Released, ControlledBowler, &ABowlerPawn::ReleaseBall);

		InputComponent->BindAction("ToggleMovement", IE_Released, ControlledBowler, &ABowlerPawn::ToggleMovementMode);

		// InputComponent->BindAxis("YBallMovement", ControlledBowler, &ABowlerPawn::MoveBallY);
		// InputComponent->BindAxis("XBallMovement", ControlledBowler, &ABowlerPawn::MoveBallX);

		InputComponent->BindTouch(IE_Pressed, this, &ABowlerPlayerController::HandleTouchPress);
		InputComponent->BindTouch(IE_Released, this, &ABowlerPlayerController::HandleTouchRelease);
		InputComponent->BindTouch(IE_Repeat, this, &ABowlerPlayerController::HandleTouchHeld);
		TouchTimerInstance = Cast<UTouchTimer>(CreateWidget(this, TouchTimerClass));
		TouchTimerInstance->AddToViewport();
	}
	
}

void ABowlerPlayerController::AttemptMoveX(float value)
{
	if (GetStateName() == NAME_Spectating || !ABowlingGameStateBase::IsActivePlayer(this))
	{
		return;
	}
	if(CurrentContinuousMove == 0.0f)
	{
		ControlledBowler->MoveX(value);
	}
}

void ABowlerPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetStateName() == NAME_Spectating || !ABowlingGameStateBase::IsActivePlayer(this))
	{
		return;
	}
	if(CurrentContinuousMove != 0.0f)
	{
		ControlledBowler->MoveX(CurrentContinuousMove);
	}
	if(bPressing)
	{
		if(MotionControlsEnabled)
		{
			FVector Tilt;
			FVector RotationRate;
			FVector Gravity;
			FVector Acceleration;
			GetInputMotionState(Tilt, RotationRate, Gravity, Acceleration);

			ControlledBowler->MoveBallY(RotationRate.X * 2);
			ControlledBowler->MoveBallX(RotationRate.Y * 4);
		}
		HoldTimeElapsed += DeltaSeconds;
		if(HoldTimeElapsed > TimeToGrip && !ControlledBowler->BallGripped)
		{
			ControlledBowler->GripBall();
		}
		if(!ControlledBowler->BallGripped)
		{
			TouchTimerInstance->UpdateTimerProgress(HoldTimeElapsed / TimeToGrip);
		}
		else
		{
			TouchTimerInstance->UpdateTimerProgress(0);
		}
	}
}

void ABowlerPlayerController::HandleTouchPress(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	if (GetStateName() == NAME_Spectating || !ABowlingGameStateBase::IsActivePlayer(this))
	{
		return;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Touch Press: %d Location: %s"), touchIndex, *location.ToString()));
	// ControlledBowler->GripBall();
	bPressing = true;
	// Update cached setting so we dont have to do a cast every frame
	MotionControlsEnabled = UBowlingGameUserSettings::GetBowlingGameUserSettings()->GetMotionControlsEnabled();
	bShowMouseCursor = false;
	HoldTimeElapsed = 0.0f;
	TouchTimerInstance->UpdatePosition(FVector2D(location));
	LastHoldPosition = location;
}

void ABowlerPlayerController::HandleTouchRelease(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	if (GetStateName() == NAME_Spectating || !ABowlingGameStateBase::IsActivePlayer(this))
	{
		return;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,
	//                                  FString::Printf(TEXT("Touch Release: %d Location: %s"), touchIndex, *location.ToString()));
	HoldTimeElapsed = 0.0f;
	ControlledBowler->ReleaseBall();
	bShowMouseCursor = true;
	LastHoldPosition = NullPos;
	bPressing = false;
	TouchTimerInstance->UpdateTimerProgress(0);
}

void ABowlerPlayerController::HandleTouchHeld(ETouchIndex::Type touchIndex, UE::Math::TVector<double> location)
{
	if (GetStateName() == NAME_Spectating || !ABowlingGameStateBase::IsActivePlayer(this))
	{
		return;
	}
	TouchTimerInstance->UpdatePosition(FVector2D(location));
	if(HoldTimeElapsed < TimeToGrip)
	{
		return;
	}

	if(LastHoldPosition == NullPos)
	{
		LastHoldPosition = location;
	}

	int screenSizeX;
	int screenSizeY;
	GetViewportSize(screenSizeX, screenSizeY);
	float xPercentageMoved = (LastHoldPosition - location).X / screenSizeX;
	float yPercentageMoved = (LastHoldPosition - location).Y / screenSizeY;
	float totalY = ControlledBowler->MaxArmAngle - ControlledBowler->MinArmAngle;
	float totalX = ControlledBowler->MaxBallSpin / 2;

	if(!MotionControlsEnabled)
	{
		ControlledBowler->MoveBallX(xPercentageMoved * 1 * totalX * -1);
		ControlledBowler->MoveBallY(yPercentageMoved * 3 * totalY);
	}
	LastHoldPosition = location;
}

void ABowlerPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	OnBeginPlayingState();
}

void ABowlerPlayerController::StartContinuousMove(float Direction)
{
	CurrentContinuousMove = Direction;
}

void ABowlerPlayerController::StopContinuousMove()
{
	CurrentContinuousMove = 0.0f;
}

void ABowlerPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	
}
