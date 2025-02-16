// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BowlingUtilFunctionLibrary.generated.h"

/**
 * A function library containing utilities for Bowling
 */
UCLASS()
class BOWLING_API UBowlingUtilFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Set view target with blend for all currently active player controllers
	UFUNCTION(BlueprintCallable, Category = "BowlingUtils")
	static void SetViewTargetWithBlendForAllPlayerControllers(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing);
};
