#include "FDueTweenAction.h"

void FDueTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	if (ActiveDueTween == INVALID_DUETWEEN_HANDLE)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	Response.FinishAndTriggerIf(FDUETweenModule::Get().GetTweenFromHandle(ActiveDueTween)->Status != EDUETweenStatus::Running, ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
