// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
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
	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueMove(UObject* Target,
	                    FLatentActionInfo LatentInfo,
	                    float Duration,
	                    FVector TargetLocation,
	                    EDueEasingType DueEasingType,
	                    int& OutHandle,
	                    int32 Steps = 0);

	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotate(UObject* Target,
	                      FLatentActionInfo LatentInfo,
	                      float Duration,
	                      FRotator TargetRotation,
	                      EDueEasingType DueEasingType,
	                      int32 Steps = 0);

	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueFloatField(UObject* Target,
	                          FLatentActionInfo LatentInfo,
	                          FName FieldName,
	                          float Duration,
	                          float TargetValue,
	                          EDueEasingType DueEasingType,
	                          int32 Steps = 0);

	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueDoubleField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           double TargetValue,
	                           EDueEasingType DueEasingType,
	                           int32 Steps = 0);

	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueVectorField(UObject* Target,
	                           FLatentActionInfo LatentInfo,
	                           FName FieldName,
	                           float Duration,
	                           FVector TargetValue,
	                           EDueEasingType DueEasingType,
	                           int32 Steps = 0);

	UFUNCTION(BlueprintCallable, Category = "DUETween",
		meta = (Latent, LatentInfo = "LatentInfo", DefaultToSelf="Target", Duration="1.0f"))
	static void DueRotatorField(UObject* Target,
	                            FLatentActionInfo LatentInfo,
	                            FName FieldName,
	                            float Duration,
	                            FRotator TargetValue,
	                            EDueEasingType DueEasingType,
	                            int32 Steps = 0);

private:
	static FActiveDueTweenHandle CreateAndStartLatentAction(UWorld* World, const FLatentActionInfo& LatentInfo, const FDUETweenData&);
};
