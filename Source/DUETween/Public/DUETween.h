#pragma once

#include "CoreMinimal.h"
#include "DUEEasingFunctionLibrary.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

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
typedef int FActiveDueTweenHandle;
constexpr FActiveDueTweenHandle INVALID_DUETWEEN_HANDLE = -1;


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

struct FActiveDueTween
{
    FDUETweenData TweenData;

    // Constant state
    FActiveDueTweenHandle Handle;
    
    // Per-Tween State
    unsigned int ID = 0; 
    float TimeElapsed = 0;
    EDUETweenStatus Status = EDUETweenStatus::Unset;
    FValueContainer StartingValue;
    union
    {
        FActiveDueTweenHandle NextFreeTween;
        struct ActiveNode
        {
            FActiveDueTweenHandle NextActiveTween;
            FActiveDueTweenHandle LastActiveTween;
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
    // Creates and starts a new tween
    DUETWEEN_API FActiveDueTweenHandle AddTween(const FDUETweenData& TweenData);
    // Gets a pointer to a tween from its handle DO NOT SAVE THIS POINTER
    DUETWEEN_API FActiveDueTween* GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle);

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
        return ActiveTweenChainStart != INVALID_DUETWEEN_HANDLE;
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
    void ReturnTweenToPool(FActiveDueTweenHandle tween);
    void TickTween(float deltaTime, FActiveDueTween* currentTween);
    void InitTweenPool();
    void ExpandPool(int Amount);
    // TODO: make this configurable
    const int INITIAL_POOL_SIZE = 10;
    int TWEEN_POOL_SIZE = INITIAL_POOL_SIZE;
    FActiveDueTween* TweenPool = nullptr;
    FActiveDueTweenHandle NextAvailableTween = INVALID_DUETWEEN_HANDLE;
    FActiveDueTweenHandle ActiveTweenChainStart = INVALID_DUETWEEN_HANDLE;
    unsigned int LastAssignedTweenId = 0;
};
