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
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

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
};
