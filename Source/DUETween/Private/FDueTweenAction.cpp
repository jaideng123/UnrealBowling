#include "FDueTweenAction.h"

void FDueTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if (ActiveDueTween == NULL_DUETWEEN_HANDLE)
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}

	FActiveDueTween* Tween = FDUETweenModule::Get().GetTweenFromHandle(ActiveDueTween);

	Response.FinishAndTriggerIf(Tween->Status == EDUETweenStatus::Completed || Tween->Status == EDUETweenStatus::Unset,
	                            ExecutionFunction, OutputLink,
	                            CallbackTarget);
}
