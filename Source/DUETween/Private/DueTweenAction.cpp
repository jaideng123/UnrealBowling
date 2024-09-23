#include "DueTweenAction.h"

void FDueTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (ActiveDueTween == NULL_DUETWEEN_HANDLE)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	FActiveDueTween* Tween = CallbackTarget.Get()->GetWorld()->GetSubsystem<UDueTweenSubsystem>()->
	                                        GetTweenFromHandle(ActiveDueTween);

	Response.FinishAndTriggerIf(Tween->Status == EDueTweenStatus::Completed || Tween->Status == EDueTweenStatus::Unset,
	                            ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
