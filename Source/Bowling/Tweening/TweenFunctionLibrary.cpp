﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenFunctionLibrary.h"

#include "FFloatTweenAction.h"
#include "FMoveTweenAction.h"
#include "FRotateTweenAction.h"
#include "UObject/UnrealTypePrivate.h"

void UTweenFunctionLibrary::DueMove(UObject*          Target,
                                    FLatentActionInfo LatentInfo,
                                    float             Duration,
                                    FVector           TargetLocation,
                                    EEasingFunc::Type EasingType,
                                    float             BlendExp,
                                    int32             Steps)
{
	// Prepare latent action
	if(UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if(LatentActionManager.FindExistingAction<FMoveTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FMoveTweenData tweenData;
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.TargetLocation = TargetLocation;
			tweenData.EasingType = EasingType;
			tweenData.Steps = Steps;
			tweenData.BlendExp = BlendExp;

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FMoveTweenAction(LatentInfo, tweenData));
		}
	}
}

void UTweenFunctionLibrary::DueRotate(UObject*          Target, FLatentActionInfo LatentInfo, float Duration, FRotator TargetRotation,
                                      EEasingFunc::Type EasingType, float         BlendExp, int32   Steps)
{
	// Prepare latent action
	if(UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if(LatentActionManager.FindExistingAction<FRotateTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FRotateTweenData tweenData;
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = EasingType;
			tweenData.Steps = Steps;
			tweenData.BlendExp = BlendExp;
			tweenData.TargetRotation = TargetRotation;

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FRotateTweenAction(LatentInfo, tweenData));
		}
	}
}

void UTweenFunctionLibrary::DueFloatField(UObject*          Target, FLatentActionInfo LatentInfo,
                                          FName             FieldName, float          Duration, float TargetValue,
                                          EEasingFunc::Type EasingType, float         BlendExp, int32 Steps)
{
	// Prepare latent action
	if(UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if(LatentActionManager.FindExistingAction<FFloatTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FFloatTweenData tweenData;
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = EasingType;
			tweenData.Steps = Steps;
			tweenData.BlendExp = BlendExp;
			
			tweenData.FieldName = FieldName;
			tweenData.TargetValue = TargetValue;

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FFloatTweenAction(LatentInfo, tweenData));
		}
	}
}