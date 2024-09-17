#include "FDueTweenAction.h"

void FDueTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	Response.FinishAndTriggerIf(ActiveDueTween->Status != EDUETweenStatus::Running, ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
