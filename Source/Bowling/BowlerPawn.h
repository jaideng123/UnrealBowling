// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallBase.h"
#include "GameFramework/Pawn.h"
#include "BowlerPawn.generated.h"

UCLASS()
class BOWLING_API ABowlerPawn : public APawn
{
	GENERATED_BODY()


public:

	// Components
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* StaticMeshRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDecalComponent* GuideDecalComp;

	// Tuning Variables

	// Which ball to spawn and throw
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABallBase> BallClass = ABallBase::StaticClass();

	// Where to spawn + hold ball relative to player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BallSpawnOffset = FVector(0);

	// Force multiplier for velocity of ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BallReleaseMultiplier = 10.0f;

	// Force multiplier for spin of ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BallSpinMultiplier = 10.0f;

	// Max velocity for ball on release
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBallForce = 1000.0f;

	// Max angle of ball spin on release
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBallSpin = 45.0f;

	// Max angle of bowler's arm while holding ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxArmAngle = 110.0f;

	// Min angle of bowler's arm while holding ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinArmAngle = -110.0f;

	// Length between pivot of arm and the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmLength = 60.0f;

	// Increases decay of throw force over time
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrowForceDecay = 2.0f;

	// Limits the lateral movement of the bowler
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementLimit = 100.0f;

	// State Variables

	// Ball currently held
	UPROPERTY(VisibleAnywhere)
	ABallBase* CurrentBall;

	// Is ball being gripped? (being wound up)
	UPROPERTY(VisibleInstanceOnly)
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

	// Current time of throw
	UPROPERTY(VisibleInstanceOnly)
	float ThrowTime = 0.0f;

	// Starting position when the player spawns
	UPROPERTY(VisibleInstanceOnly)
	FVector StartingPosition = FVector(0);
	
	
	// Sets default values for this pawn's properties
	ABowlerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveX(float value);
	void MoveBallY(float value);
	void MoveBallX(float value);
	void GripBall();
	void ReleaseBall();
	
	UFUNCTION(BlueprintCallable)
	void SpawnNewBall();
private:
	float CalculateReleaseForce() const;
};
