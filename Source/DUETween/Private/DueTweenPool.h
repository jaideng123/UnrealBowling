#pragma once
#include "ActiveDueTween.h"

/**
 * Pool that manages a heap-allocated array of tweens
 */
class FDueTweenPool
{
public:
	// Tween Pool Methods
	void InitTweenPool();
	void ExpandPool(const int& Amount);
	FActiveDueTween* GetTweenFromHandle(FActiveDueTweenHandle NextAvailableTween) const;
	FActiveDueTweenHandle GetTweenFromPool();
	void ReturnTweenToPool(FActiveDueTweenHandle TweenToReturnHandle);
	int GetCurrentPoolSize() const;

private:
	int CurrentTotalPoolSize = 0;
	int CurrentUnusedElements = 0;
	FActiveDueTween* TweenPool = nullptr;
	FActiveDueTweenHandle NextAvailableTween = NULL_DUETWEEN_HANDLE;
};
