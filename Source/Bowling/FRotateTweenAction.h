
#pragma once
#include "LatentActions.h"
#include "Kismet/KismetMathLibrary.h"
#include "FRotateTweenAction.generated.h"


USTRUCT()
struct FRotateTweenData {
	GENERATED_BODY()
public:
	UPROPERTY()
	UObject* Target;
	UPROPERTY()
	float Duration;
	UPROPERTY()
	FRotator TargetRotation;
	UPROPERTY()
	TEnumAsByte<EEasingFunc::Type> EasingType;
	UPROPERTY()
	float BlendExp;
	UPROPERTY()
	int32 Steps;
};


// FTweenAction
// Tweens from one location to another over a duration
class FRotateTweenAction : public FPendingLatentAction {
public:
	FName          ExecutionFunction;
	int32          OutputLink;
	FWeakObjectPtr CallbackTarget;
	FRotateTweenData     TweenData;

private:
	float   TimeElapsed;
	FRotator StartingRotation;

public:
	FRotateTweenAction(const FLatentActionInfo& LatentInfo, FRotateTweenData TweenData)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
		  , TweenData(TweenData)
		  , TimeElapsed(0)
	{
		if(const AActor* targetAsActor = Cast<AActor>(TweenData.Target))
		{
			StartingRotation = targetAsActor->GetActorRotation();
		}
		if(const USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target))
		{
			StartingRotation = targetAsSceneComponent->GetRelativeRotation();
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
			NSLOCTEXT("RotateTweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(TweenData.Duration - TimeElapsed, &DelayTimeFormatOptions)).ToString();
	}
#endif
};
