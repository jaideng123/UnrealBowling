// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "LatentActions.h"
#include "Engine/Internal/Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"
#include "FTweenAction.generated.h"


USTRUCT()
struct FTweenData {
	GENERATED_BODY()
public:
	UPROPERTY()
	UObject* Target;
	UPROPERTY()
	float Duration;
	UPROPERTY()
	FVector TargetLocation;
	UPROPERTY()
	TEnumAsByte<EEasingFunc::Type> EasingType;
	UPROPERTY()
	float BlendExp;
	UPROPERTY()
	int32 Steps;
};


// FTweenAction
// Tweens from one location to another over a duration
class FTweenAction : public FPendingLatentAction {
public:
	FName          ExecutionFunction;
	int32          OutputLink;
	FWeakObjectPtr CallbackTarget;
	FTweenData     TweenData;

private:
	float   TimeElapsed;
	FVector StartingLocation;

public:
	FTweenAction(const FLatentActionInfo& LatentInfo, FTweenData TweenData)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
		  , TweenData(TweenData)
		  , TimeElapsed(0)
	{
		if(const AActor* targetAsActor = Cast<AActor>(TweenData.Target))
		{
			StartingLocation = targetAsActor->GetActorLocation();
		}
		if(const USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target))
		{
			StartingLocation = targetAsSceneComponent->GetRelativeLocation();
		}
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		static const FNumberFormattingOptions DelayTimeFormatOptions = FNumberFormattingOptions()
		                                                               .SetMinimumFractionalDigits(3)
		                                                               .SetMaximumFractionalDigits(3);
		return FText::Format(
			NSLOCTEXT("TweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(TweenData.Duration - TimeElapsed, &DelayTimeFormatOptions)).ToString();
	}
#endif
};
