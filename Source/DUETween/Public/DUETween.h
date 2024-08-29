#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "DUETween.generated.h"


enum class EasingType : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);

USTRUCT()
struct FActiveDueTween
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TWeakObjectPtr<UObject> Target;
    UPROPERTY()
    float Duration;
    UPROPERTY()
    TEnumAsByte<EasingType> EasingType;
    UPROPERTY()
    float BlendExp;
    UPROPERTY()
    int32 Steps;
};

class FDUETweenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
private:
    bool Tick(float deltaTime);
    FTickerDelegate TickDelegate;
    FTSTicker::FDelegateHandle TickDelegateHandle;
    TArray<FActiveDueTween> ActiveTweens;
};
