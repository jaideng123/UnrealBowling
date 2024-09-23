#pragma once

#include "CoreMinimal.h"
#include "DueEasingFunctionLibrary.h"
#include "ActiveDueTween.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);
DECLARE_STATS_GROUP(TEXT("DUETWEEN"), STATGROUP_DUETWEEN, STATCAT_Advanced);


/**
 * Main Module for Due Tween
 */
class FDUETweenModule : public IModuleInterface
{
public:
	// Get active DueTween Module Instance
	static FDUETweenModule& Get();
	
	// IModuleInterface Methods
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
