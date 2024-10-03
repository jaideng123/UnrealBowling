#pragma once
#include "DUETweenModule.h"
#include "DueEasingFunctionLibrary.h"
#include "DueTweenSubsystem.h"
#include "LatentActions.h"


/**
 * Latent Action that tracks an active tween
 */
class DUETWEEN_API FDueTweenAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

private:
	FActiveDueTweenHandle ActiveDueTween;

public:
	FDueTweenAction(const FLatentActionInfo& LatentInfo, const FActiveDueTweenHandle& NewActiveDueTweenHandle)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
	{
		ActiveDueTween = NewActiveDueTweenHandle;
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		static const FNumberFormattingOptions DelayTimeFormatOptions = FNumberFormattingOptions()
		                                                               .SetMinimumFractionalDigits(3)
		                                                               .SetMaximumFractionalDigits(3);
		FActiveDueTween* tween = CallbackTarget.Get()->GetWorld()->GetSubsystem<UDueTweenSubsystem>()->
		                                        GetTweenFromHandle(ActiveDueTween);
		return FText::Format(
			NSLOCTEXT("DueTweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(
				tween->TweenData.Duration - CallbackTarget.Get()->GetWorld()->GetSubsystem<UDueTweenSubsystem>()->
				                                           GetTweenFromHandle(ActiveDueTween)->TimeElapsed,
				&DelayTimeFormatOptions)).ToString();
	}
#endif
};
