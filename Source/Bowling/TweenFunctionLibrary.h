// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TweenFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BOWLING_API UTweenFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "DueTween", meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueMove(UObject*          Target,
	                    FLatentActionInfo LatentInfo,
	                    float             Duration,
	                    FVector           TargetLocation,
	                    EEasingFunc::Type EasingType,
	                    float             BlendExp = 2,
	                    int32             Steps = 2);
};
