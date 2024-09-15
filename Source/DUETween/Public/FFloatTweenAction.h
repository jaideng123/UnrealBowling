#pragma once
#include "DUETween.h"
#include "DUEEasingFunctionLibrary.h"
#include "FTweenData.h"
#include "LatentActions.h"




// FTweenAction
// Tweens from one location to another over a duration
class DUETWEEN_API FFloatTweenAction : public FPendingLatentAction {
public:
	FName           ExecutionFunction;
	int32           OutputLink;
	FWeakObjectPtr  CallbackTarget;
	FDUETweenData TweenData;

private:
	FActiveDueTween* ActiveDueTween;

public:
	FFloatTweenAction(const FLatentActionInfo& LatentInfo, FDUETweenData TweenData)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
		  , TweenData(TweenData)
	{
		ActiveDueTween = FDUETweenModule::Get().AddTween(TweenData);
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
			NSLOCTEXT("FloatTweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(TweenData.Duration - ActiveDueTween->TimeElapsed, &DelayTimeFormatOptions)).ToString();
	}
#endif
};
