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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABallBase> BallClass = ABallBase::StaticClass();
	UPROPERTY(VisibleAnywhere)
	ABallBase* CurrentBall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BallSpawnOffset = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* StaticMeshRoot;

	UPROPERTY(VisibleInstanceOnly)
	bool BallGripped = false;

	UPROPERTY(VisibleInstanceOnly)
	float BallRotationOffset = 0.0f;

	UPROPERTY(VisibleInstanceOnly)
	float ThrowDistance = 0.0f;

	UPROPERTY(VisibleInstanceOnly)
	float ThrowTime = 0.0f;

	UPROPERTY(VisibleInstanceOnly)
	float LastInput = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BallReleaseMultiplier = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxBallForce = 1000.0f;
	
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
	void GripBall();
	void ReleaseBall();
private:
	float CalculateReleaseForce() const;
};
