// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveDUETweenHandle.h"
#include "BallBase.h"
#include "GameFramework/Pawn.h"
#include "BowlerPawn.generated.h"

UENUM(BlueprintType)
enum EBowlerMovementMode { MOVE, ROTATE };

UCLASS()
class BOWLING_API ABowlerPawn : public APawn {
	GENERATED_BODY()

public:
	// Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* StaticMeshRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDecalComponent* GuideDecalComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* BallPivotComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* BallHandComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* BallAnchorComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* MoveModeDisplayComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* RotateModeDisplayComp;

	// Tuning Variables

	// Which ball to spawn and throw
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABallBase> BallClass = ABallBase::StaticClass();

	// Where to spawn + hold ball relative to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BallSpawnOffset = FVector(0);

	// Force multiplier for velocity of ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float BallReleaseMultiplier = 10.0f;

	// Force multiplier for spin of ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float BallSpinMultiplier = 10.0f;

	// Max velocity for ball on release
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MaxBallForce = 1000.0f;

	// Max velocity for ball on release
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MinBallForce = 100.0f;

	// Max angle of ball spin on release
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MaxBallSpin = 45.0f;

	// Max angle of bowler's arm while holding ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MaxArmAngle = 110.0f;

	// Min angle of bowler's arm while holding ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MinArmAngle = -110.0f;

	// Max Angle to move bowlers hand to signal spin
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float MaxHandShift = 30.0f;

	// Length between pivot of arm and the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmLength = 60.0f;

	// Increases decay of throw force over time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float ThrowForceDecay = 2.0f;

	// Limits the lateral movement of the throw
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	double MaxZVelocity = 100.0f;

	// Minimum amount of force required to release the bacll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	double ReleaseForceThreshold = 10.0f;

	// Minimum amount of windup required to release the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ball Throw")
	float WindupDistanceThreshold = 20.0f;

	// How far back to start the run up sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Run Up")
	float StartDistance = 100.0f;

	// The total time the runup sequence takes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Run Up")
	float RunUpTimeMS = 1.0f;

	// Movement curve for the runup sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Run Up")
	FRuntimeFloatCurve RunUpCurve;

	// How fast to reset the bowler after the runup sequence is cancelled
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Run Up")
	float ResetSpeed = 1.0f;

	// State Variables

	// Ball currently held
	UPROPERTY(VisibleAnywhere)
	ABallBase* CurrentBall;

	// Ball currently held
	UPROPERTY(VisibleAnywhere)
	ABallBase* ThrownBall;

	// Is ball being gripped? (being wound up)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool BallGripped = false;

	// Current rotation of the ball
	UPROPERTY(VisibleInstanceOnly)
	float BallRotationOffset = 0.0f;

	// Current spin amount
	UPROPERTY(VisibleInstanceOnly)
	float BallSpinAmount = 0.0f;

	// Current distance travelled during throw
	UPROPERTY(VisibleInstanceOnly)
	float ThrowDistance = 0.0f;

	// Overall distance travelled winding up throw
	UPROPERTY(VisibleInstanceOnly)
	float ThrowWindupDistance = 0.0f;

	// Whether we have passed the threshold required for a windup
	UPROPERTY(VisibleInstanceOnly)
	bool ThrowWindupThresholdPassed = false;


	// Current time of throw
	UPROPERTY(VisibleInstanceOnly)
	float ThrowTime = 0.0f;

	// Current time of throw
	UPROPERTY(VisibleInstanceOnly)
	float GrippedTime = 0.0f;

	// Current time of throw
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool BowlingLocked = false;

	// Starting position when the player spawns
	UPROPERTY(VisibleInstanceOnly)
	FVector
	StartingPosition = FVector(0);

	// Starting rotation when the player spawns
	UPROPERTY(VisibleInstanceOnly)
	FRotator StartingOrientation = FRotator::ZeroRotator;
	UPROPERTY(VisibleInstanceOnly)
	FVector InitialForward;
	UPROPERTY(VisibleInstanceOnly)
	FVector            InitialRight;
	TOptional<FVector> BallGripStartPosition;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBowlerMovementMode> CurrentMovementMode = MOVE;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool IsZoomedIn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float StrafingSpeed = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float RotationSpeedDegrees = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MaxRotation = 30.0;

	ABowlerPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveX(float value);
	void MoveBallY(float value);
	void MoveBallX(float value);
	void GripBall();
	void ReleaseBall();
	UFUNCTION(BlueprintCallable)
	void ToggleMovementMode();

	float CalculateBallSpin();
	void  ResetBallGripState();
	void  AttachBallToHand();
	void  UpdateMovementModeDisplay() const;
	void  HideMovementModeDisplay() const;

	UFUNCTION(BlueprintCallable)
	void SpawnNewBall();

	UFUNCTION(BlueprintCallable)
	void ResetBall();

	UFUNCTION(BlueprintImplementableEvent)
	void OnNoReleaseForce();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRelease();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWindupThresholdPassed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGrip();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ZoomIn();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ZoomOut();

	UFUNCTION(BlueprintNativeEvent)
	void OnMove(float moveDist);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveReset();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HideUI();
	UFUNCTION(BlueprintCallable)
	void ShowUI();
	UFUNCTION(BlueprintCallable)
	void ShowPreBowlUI();
	UFUNCTION(BlueprintCallable)
	void ShowControlUI();
	UFUNCTION(BlueprintCallable)
	void ShowEndUI();
	void PossiblyStartRunUpTween();
	void CancelRunUpTween();

private:
	float CalculateReleaseForce() const;

	FActiveDUETweenHandle MoveTweenHandle;

	
};
