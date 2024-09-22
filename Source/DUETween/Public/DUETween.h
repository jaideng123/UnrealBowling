#pragma once

#include "CoreMinimal.h"
#include "DueEasingFunctionLibrary.h"
#include "ActiveDueTween.h"
#include "Containers/Union.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDUETween, Log, All);
DECLARE_STATS_GROUP(TEXT("DUETWEEN"), STATGROUP_DUETWEEN, STATCAT_Advanced);

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
