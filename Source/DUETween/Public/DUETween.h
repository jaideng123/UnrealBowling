#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDUETweenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
private:
    bool Tick(float deltaTime);
    FTickerDelegate TickDelegate;
    FTSTicker::FDelegateHandle TickDelegateHandle;
};
