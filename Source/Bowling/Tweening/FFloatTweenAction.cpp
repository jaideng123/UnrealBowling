#include "FFloatTweenAction.h"

void FFloatTweenAction::UpdateOperation(FLatentResponse& Response)
{

	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	// Leaving this for now to validate
	// TimeElapsed += Response.ElapsedTime();
	// const float percentageComplete = FMath::Clamp(TimeElapsed / TweenData.Duration, 0, 1);
	// UE_LOG(LogTemp, Display, TEXT("Ideal Percentage Complete: %f"), percentageComplete);
	// const float newValue = UKismetMathLibrary::Ease(StartingValue, TweenData.TargetValue, percentageComplete, EEasingFunc::Linear,
	//                                                 TweenData.BlendExp, TweenData.Steps);
	// UE_LOG(LogTemp, Display, TEXT("Ideal Value: %f"), newValue);
	// UE_LOG(LogTemp, Display, TEXT("Latent DeltaTime: %f"), Response.ElapsedTime());
	
	Response.FinishAndTriggerIf(!ActiveDueTween->IsActive, ExecutionFunction, OutputLink, CallbackTarget);
}
