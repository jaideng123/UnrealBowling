#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

#include "DUETween.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);

enum class EDUEValueType
{
    Float,
    Double,
    Vector,
    Rotator,
};

typedef TUnion<FVector,FRotator,float,double> FValueContainer;

USTRUCT()
struct FDUETweenData
{
    GENERATED_BODY()
public:
    TWeakObjectPtr<> Target;
    FProperty* TargetProperty;
    float Duration;
    EDueEasingType EasingType;
    EDUEValueType ValueType;
    FValueContainer TargetValue;
    int32 Steps;
};

USTRUCT()
struct FActiveDueTween
{
    GENERATED_BODY()
public:
    FDUETweenData TweenData;
    // State
    FValueContainer StartingValue;
    float TimeElapsed = 0;
    bool IsActive = false;
};

class FDUETweenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    FValueContainer GetCurrentValueFromProperty(const FDUETweenData& TweenData);
    void SetCurrentValueToProperty(const FDUETweenData& TweenData, FValueContainer newValue);
    // DUETWEEN_API is necessary to expose this method
    DUETWEEN_API FActiveDueTween* AddTween(const FDUETweenData& TweenData);
    static FDUETweenModule& Get()
    {
        return FModuleManager::LoadModuleChecked< FDUETweenModule >( "DUETween" );
    }
private:
    bool Tick(float deltaTime);
    FTickerDelegate TickDelegate;
    FTSTicker::FDelegateHandle TickDelegateHandle;
    TArray<FActiveDueTween> ActiveTweens;
};
