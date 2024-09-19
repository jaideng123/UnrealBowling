#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

#include "DUETween.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);

enum class EDUETweenStatus
{
    // This tween is unused and ready to be reclaimed
    Unset,
    // This tween is running
    Running,
    // This tween is completed and will be marked unused next frame
    Completed,
    // TODO: Support pausing tweens
};

enum class EDUEValueType
{
    Float,
    Double,
    Vector,
    Rotator,
};

typedef TUnion<FVector,FRotator,float,double> FValueContainer;

// Data that defines the tween
struct FDUETweenData
{
    TWeakObjectPtr<> Target;
    FProperty* TargetProperty;
    float Duration = 0;
    EDueEasingType EasingType = Linear;
    EDUEValueType ValueType = EDUEValueType::Float;
    int32 Steps = 0;
    FValueContainer TargetValue;
};

USTRUCT()
struct FActiveDueTween
{
    GENERATED_BODY()
public:
    FDUETweenData TweenData;

    // State
    unsigned int ID = 0; 
    float TimeElapsed = 0;
    EDUETweenStatus Status = EDUETweenStatus::Unset;
    FValueContainer StartingValue;
    union
    {
        FActiveDueTween* NextFreeTween;
        struct ActiveNode
        {
            FActiveDueTween* NextActiveTween;
            FActiveDueTween* LastActiveTween;
        };
        ActiveNode ActiveNode;
    } TweenPtr;
};

// TODO Try:
// , public FTickableGameObject
class FDUETweenModule : public IModuleInterface, public FTickableGameObject
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
    
    //FTickableGameObject interfaces
    virtual UWorld* GetTickableGameObjectWorld() const override 
    { 
        return nullptr; 
    }
    virtual ETickableTickType GetTickableTickType() const override 
    {
        return ETickableTickType::Conditional; 
    }
    virtual bool IsTickable() const override final 
    {
        return true;
    }
    virtual void Tick(float deltaTime) override;
    virtual TStatId GetStatId() const override { return TStatId(); }
    
private:
    void ReturnTweenToPool(FActiveDueTween* tween);
    void TickTween(float deltaTime, FActiveDueTween* currentTween);
    void InitTweenPool();
    FTickerDelegate TickDelegate;
    FTSTicker::FDelegateHandle TickDelegateHandle;
    static const bool USE_ARRAY = true;
    static constexpr int TWEEN_POOL_SIZE = 1000;
    FActiveDueTween TweenPool[TWEEN_POOL_SIZE] = {};
    FActiveDueTween* NextAvailableTween = nullptr;
    FActiveDueTween* ActiveTweenChainStart = nullptr;
    unsigned int LastAssignedTweenId = 0;
    int ActiveTweenCount = 0;
};
