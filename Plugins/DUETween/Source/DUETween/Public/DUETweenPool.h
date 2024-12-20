#pragma once
#include "ActiveDUETweenHandle.h"

struct FActiveDUETween;
LLM_DECLARE_TAG(FDUETweenPoolTag);

/**
 * Pool that manages a heap-allocated array of tweens
 */
class FDUETweenPool
{
public:
	// Tween Pool Methods
	void InitTweenPool();
	void ExpandPool(const int& Amount);
	FActiveDUETween* GetTweenFromHandle(const FActiveDUETweenHandle& TweenHandle, const bool& CheckVersion = true) const;
	FActiveDUETweenHandle GetTweenFromPool(const TWeakObjectPtr<UWorld>& World);
	void ReturnTweenToPool(FActiveDUETweenHandle TweenToReturnHandle);
	int GetCurrentPoolSize() const;
	void Deinitialize();

private:
	TArray<int> Test;
	int CurrentTotalPoolSize = 0;
	int CurrentUnusedElements = 0;
	FActiveDUETween* TweenPool = nullptr;
	FActiveDUETweenHandle NextAvailableTween;
	void ClearPool(FActiveDUETween*& Pool);
	FActiveDUETween* ReallocatePool(FActiveDUETween* OldPool, const int& NewPoolSize);
};
