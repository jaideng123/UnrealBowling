#include "FMoveTweenAction.h"

void FMoveTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	TimeElapsed += Response.ElapsedTime();
	const float   percentageComplete = FMath::Clamp(TimeElapsed / TweenData.Duration, 0, 1);
	const FVector newLocation = UKismetMathLibrary::VEase(StartingLocation, TweenData.TargetLocation, percentageComplete, TweenData.EasingType, TweenData.BlendExp, TweenData.Steps);
	if(AActor* targetAsActor = Cast<AActor>(TweenData.Target))
	{
		targetAsActor->SetActorLocation(newLocation);
	}
	if(USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target))
	{
		targetAsSceneComponent->SetRelativeLocation(newLocation);
	}
	Response.FinishAndTriggerIf(TweenData.Duration - TimeElapsed <= 0.0f, ExecutionFunction, OutputLink, CallbackTarget);
}
