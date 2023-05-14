// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BowlerPawn.h"
#include "GameFramework/PlayerController.h"
#include "BowlerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API ABowlerPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	void         HandleTouchPress(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	void         HandleTouchRelease(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	void         HandleTouchHeld(ETouchIndex::Type arg, UE::Math::TVector<double> vector);
	ABowlerPawn* ControlledBowler;
};
