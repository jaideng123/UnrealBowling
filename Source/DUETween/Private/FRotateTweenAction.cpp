#include "FRotateTweenAction.h"

void FRotateTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	TimeElapsed += Response.ElapsedTime();
	const float   percentageComplete = FMath::Clamp(TimeElapsed / TweenData.Duration, 0, 1);
	const FRotator newRotation = UKismetMathLibrary::REase(StartingRotation, TweenData.TargetRotation, percentageComplete, true, TweenData.EasingType,
	                                                      TweenData.BlendExp, TweenData.Steps);
	if(AActor* targetAsActor = Cast<AActor>(TweenData.Target))
	{
		targetAsActor->SetActorRotation(newRotation);
	}
	if(USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target))
	{
		targetAsSceneComponent->SetRelativeRotation(newRotation);
	}
	Response.FinishAndTriggerIf(TweenData.Duration - TimeElapsed <= 0.0f, ExecutionFunction, OutputLink, CallbackTarget);
}
