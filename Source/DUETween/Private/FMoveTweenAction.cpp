#include "FMoveTweenAction.h"

void FMoveTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	Response.FinishAndTriggerIf(!ActiveDueTween->IsActive, ExecutionFunction, OutputLink, CallbackTarget);
}
