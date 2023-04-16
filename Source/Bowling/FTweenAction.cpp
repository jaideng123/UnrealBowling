#include "FTweenAction.h"

void FTweenAction::UpdateOperation(FLatentResponse& Response)
{
	TimeElapsed += Response.ElapsedTime();
	const float   percentageComplete = FMath::Clamp(TimeElapsed / TweenData.Duration, 0, 1);
	const FVector newLocation = UKismetMathLibrary::VEase(StartingLocation, TweenData.TargetLocation, percentageComplete, TweenData.EasingType);
	if(AActor* targetAsActor = Cast<AActor>(TweenData.Target))
	{
		targetAsActor->SetActorLocation(newLocation);
	}
	Response.FinishAndTriggerIf(TweenData.Duration - TimeElapsed <= 0.0f, ExecutionFunction, OutputLink, CallbackTarget);
}
