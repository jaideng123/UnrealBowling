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
	// This tween is paused
	Paused,
	// This tween is being fast-forwarded to it's end point
	FastForward,
};

enum class EDUEValueType
{
	Float,
	Double,
	Vector,
	Rotator,
};

typedef TUnion<FVector, FRotator, float, double> FValueContainer;
typedef int FActiveDueTweenHandle;
constexpr FActiveDueTweenHandle NULL_DUETWEEN_HANDLE = -1;


// Data that defines the characteristics of the tween
struct FDUETweenData
{
	FProperty* TargetProperty;
	TWeakObjectPtr<> Target;
	FValueContainer TargetValue;
	float Duration = 0;
	EDueEasingType EasingType = EDueEasingType::Linear;
	EDUEValueType ValueType = EDUEValueType::Float;
	int32 Steps = 0;
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

		struct FActiveNode
		{
			FActiveDueTweenHandle NextActiveTween;
			FActiveDueTweenHandle LastActiveTween;
		};

		FActiveNode ActiveNode;
	} TweenPtr;
};

// TODO Try:
// UGameInstanceSubsystem https://benui.ca/unreal/subsystem-singleton/
class FDUETweenModule : public IModuleInterface
{
public:
	// Creates and starts a new tween
	DUETWEEN_API FActiveDueTweenHandle AddTween(const FDUETweenData& TweenData);
	// Pauses a currently running tween (returns true if successful)
	DUETWEEN_API bool PauseTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Resumes a paused tween (returns true if successful)
	DUETWEEN_API bool ResumeTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Skips to the end of a tween (returns true if successful)
	DUETWEEN_API bool FastForwardTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Skips to the end of a tween (returns true if successful)
	DUETWEEN_API bool StopTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Gets a pointer to a tween from its handle DO NOT SAVE THIS POINTER
	FActiveDueTween* GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle) const;
	// Get active DueTween Module Instance
	static FDUETweenModule& Get();

	// Tween Pool Methods
	void InitTweenPool();
	void ExpandPool(const int& Amount);
	FActiveDueTweenHandle GetTweenFromPool();
	void ReturnTweenToPool(FActiveDueTweenHandle TweenToReturnHandle);
	FActiveDueTweenHandle GetActiveTweenChainStart() const;
	void TickTween(float DeltaTime, FActiveDueTween* CurrentTween);


	// IModuleInterface Methods
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static FValueContainer GetCurrentValueFromProperty(const FDUETweenData& TweenData);
	static void SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue);
	
	int TWEEN_POOL_SIZE = 100;
	FActiveDueTween* TweenPool = nullptr;
	FActiveDueTweenHandle NextAvailableTween = NULL_DUETWEEN_HANDLE;
	FActiveDueTweenHandle ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;
	unsigned int LastAssignedTweenId = 0;
};
