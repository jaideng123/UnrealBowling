
#pragma once
#include "FTweenData.h"
#include "LatentActions.h"
#include "Kismet/KismetMathLibrary.h"
#include "FMoveTweenAction.generated.h"


USTRUCT()
struct FMoveTweenData: public FTweenData{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector TargetLocation;
};


// FTweenAction
// Tweens from one location to another over a duration
class FMoveTweenAction : public FPendingLatentAction {
public:
	FName          ExecutionFunction;
	int32          OutputLink;
	FWeakObjectPtr CallbackTarget;
	FMoveTweenData     TweenData;

private:
	float   TimeElapsed;
	FVector StartingLocation;

public:
	FMoveTweenAction(const FLatentActionInfo& LatentInfo, FMoveTweenData TweenData)
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
			NSLOCTEXT("MoveTweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(TweenData.Duration - TimeElapsed, &DelayTimeFormatOptions)).ToString();
	}
#endif
};
