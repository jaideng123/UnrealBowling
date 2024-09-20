#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

#include "DUETween.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);
DECLARE_STATS_GROUP(TEXT("DUETWEEN"), STATGROUP_DUETWEEN, STATCAT_Advanced); 

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
// UGameInstanceSubsystem https://benui.ca/unreal/subsystem-singleton/
class FDUETweenModule : public IModuleInterface, public FTickableGameObject
{
public:

    FValueContainer GetCurrentValueFromProperty(const FDUETweenData& TweenData);
    void SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& newValue);
    // DUETWEEN_API is necessary to expose this method
    DUETWEEN_API FActiveDueTween* AddTween(const FDUETweenData& TweenData);

    static FDUETweenModule& Get()
    {
        return FModuleManager::LoadModuleChecked< FDUETweenModule >( "DUETween" );
    }
    // IModuleInterface Methods
    virtual void StartupModule() override;
    void HandleWorldBeginTearDown(UWorld* World);
    virtual void ShutdownModule() override;
    
    //FTickableGameObject Methods
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
        return ActiveTweenChainStart != nullptr;
    }

    virtual bool IsTickableInEditor() const override
    {
        return false;
    }

    virtual bool IsTickableWhenPaused() const override
    {
        return false;
    }
    virtual void Tick(float deltaTime) override;
    virtual TStatId GetStatId() const override { return TStatId(); }

private:
    void ReturnTweenToPool(FActiveDueTween* tween);
    void TickTween(float deltaTime, FActiveDueTween* currentTween);
    void InitTweenPool();
    static constexpr int TWEEN_POOL_SIZE = 10000;
    FActiveDueTween TweenPool[TWEEN_POOL_SIZE] = {};
    FActiveDueTween* NextAvailableTween = nullptr;
    FActiveDueTween* ActiveTweenChainStart = nullptr;
    unsigned int LastAssignedTweenId = 0;
};
