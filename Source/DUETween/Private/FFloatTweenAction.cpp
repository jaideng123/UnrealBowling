﻿#include "FFloatTweenAction.h"

void FFloatTweenAction::UpdateOperation(FLatentResponse& Response)
{
	if(!TweenData.Target.IsValid())
	{
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		return;
	}
	
	Response.FinishAndTriggerIf(!ActiveDueTween->IsActive, ExecutionFunction, OutputLink, CallbackTarget);
}