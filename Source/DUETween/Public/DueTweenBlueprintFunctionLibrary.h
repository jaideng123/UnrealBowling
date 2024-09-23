// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DueEasingFunctionLibrary.h"
#include "DUETween.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DueTweenBlueprintFunctionLibrary.generated.h"

struct FDUETweenData;
/**
 * Old Tween Function Library
 */
UCLASS()
class DUETWEEN_API UDueTweenBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Run a tween to move an actor/component from it's current location to a target location
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueMove(UObject* Target,
	                    FLatentActionInfo LatentInfo,
	                    float Duration,
	                    FVector TargetLocation,
	                    EDueEasingType DueEasingType,
	                    int& OutHandle,
	                    int32 Steps = 0);

	// Run a tween to rotate an actor/component from it's current rotation to a target rotation
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotate(UObject* Target,
	                      FLatentActionInfo LatentInfo,
	                      float Duration,
	                      FRotator TargetRotation,
	                      EDueEasingType DueEasingType,
	                      int& OutHandle,
	                      int32 Steps = 0);

	// Run a tween that will change a float property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueFloatField(UObject* Target,
	                          FLatentActionInfo LatentInfo,
	                          FName FieldName,
	                          float Duration,
	                          float TargetValue,
	                          EDueEasingType DueEasingType,
	                          int& OutHandle,
	                          int32 Steps = 0);

	// Run a tween that will change a double property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueDoubleField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           double TargetValue,
	                           EDueEasingType DueEasingType,
	                           int& OutHandle,
	                           int32 Steps = 0);

	// Run a tween that will change a vector property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueVectorField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           FVector TargetValue,
	                           EDueEasingType DueEasingType,
	                           int& OutHandle,
	                           int32 Steps = 0);

	// Run a tween that will change a rotator property from it's current value to the target value
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotatorField(UObject* Target,
	                            FLatentActionInfo LatentInfo,
	                            FName FieldName,
	                            float Duration,
	                            FRotator TargetValue,
	                            EDueEasingType DueEasingType,
	                            int& OutHandle,
	                            int32 Steps = 0);

	// Pause an actively running tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void PauseDueTween(UObject* Target, const int& DueTweenHandle, bool& Success);
	
	// Resume a paused tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void ResumeDueTween(UObject* Target, const int& DueTweenHandle, bool& Success);

	// Fast-Forward a tween to it's end state
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void FastForwardDueTween(UObject* Target, const int& DueTweenHandle, bool& Success);

	// Stop and cancel a tween
	UFUNCTION(BlueprintCallable, Category = "DUETween", meta = (DefaultToSelf="Target"))
	static void StopDueTween(UObject* Target, const int& DueTweenHandle, bool& Success);

private:
	static FActiveDueTweenHandle CreateAndStartLatentAction(UWorld* World, const FLatentActionInfo& LatentInfo, const FDUETweenData&);
};
