#include "FFloatTweenAction.h"

void FFloatTweenAction::UpdateOperation(FLatentResponse& Response)
{

	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	TimeElapsed += Response.ElapsedTime();
	const float percentageComplete = FMath::Clamp(TimeElapsed / TweenData.Duration, 0, 1);
	const float newValue = UKismetMathLibrary::Ease(StartingValue, TweenData.TargetValue, percentageComplete, TweenData.EasingType,
	                                                TweenData.BlendExp, TweenData.Steps);
	if(FloatProperty)
	{
		FloatProperty->SetValue_InContainer(TweenData.Target.Get(), newValue);
	}
	Response.FinishAndTriggerIf(TweenData.Duration - TimeElapsed <= 0.0f, ExecutionFunction, OutputLink, CallbackTarget);
}
