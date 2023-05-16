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
	UE::Math::TVector<double> LastHoldPosition = UE::Math::TVector<double>(-1,-1,-1);
	inline static const UE::Math::TVector<double> NullPos = UE::Math::TVector<double>(-1,-1,-1);
};
