#pragma once

#include "CoreMinimal.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

#include "DUETween.generated.h"

enum class EasingType : uint8;

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);

enum class EValueType
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
    EasingType EasingType;
    EValueType ValueType;
    FValueContainer StartingValue;
    FValueContainer EndingValue;
    float BlendExp;
    int32 Steps;
};

USTRUCT()
struct FActiveDueTween
{
    GENERATED_BODY()
public:
    FDUETweenData TweenData;
    // State
    float TimeElapsed = 0;
    bool IsActive = false;
};

class FDUETweenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
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
