// Fill out your copyright notice in the Description page of Project Settings.


#include "TweenFunctionLibrary.h"

#include "FTweenAction.h"

void UTweenFunctionLibrary::DueMove(UObject*          Target,
                                    FLatentActionInfo LatentInfo,
                                    float             Duration,
                                    FVector           TargetLocation,
                                    EEasingFunc::Type EasingType,
                                    float BlendExp,
                                    int32 Steps)
{
	// Prepare latent action
	if(UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if(LatentActionManager.FindExistingAction<FTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			FTweenData tweenData;
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.TargetLocation = TargetLocation;
			tweenData.EasingType = EasingType;
			tweenData.Steps = Steps;
			tweenData.BlendExp = BlendExp;
			
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
			                                 new FTweenAction(LatentInfo, tweenData));
		}
	}
}
