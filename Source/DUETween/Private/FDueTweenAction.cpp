#include "FDueTweenAction.h"

void FDueTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (ActiveDueTween == NULL_DUETWEEN_HANDLE)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	FActiveDueTween* Tween = CallbackTarget.Get()->GetWorld()->GetSubsystem<UDueTweenSubsystem>()->
	                                        GetTweenFromHandle(ActiveDueTween);

	Response.FinishAndTriggerIf(Tween->Status == EDUETweenStatus::Completed || Tween->Status == EDUETweenStatus::Unset,
	                            ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
