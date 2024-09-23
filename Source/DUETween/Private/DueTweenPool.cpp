#include "DueTweenPool.h"

#include "DUETween.h"
#include "DueTweenSettings.h"

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Pooled Tweens"), STAT_POOLED_TWEENS, STATGROUP_DUETWEEN);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Tween Pool Size"), STAT_TWEEN_POOL_SIZE, STATGROUP_DUETWEEN);

void DueTweenPool::InitTweenPool()
{
	if (TweenPool != nullptr)
	{
		delete[] TweenPool;
	}

	check(GetDefault<UDueTweenSettings>()->InitialTweenPoolSize <= GetDefault<UDueTweenSettings>()->MaxTweenPoolSize)

	CurrentTotalPoolSize = GetDefault<UDueTweenSettings>()->InitialTweenPoolSize;

	TweenPool = new FActiveDueTween[CurrentTotalPoolSize];

	for (int i = 0; i < CurrentTotalPoolSize - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = FActiveDueTweenHandle(i + 1);
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[CurrentTotalPoolSize - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[CurrentTotalPoolSize - 1].Status = EDUETweenStatus::Unset;
	TweenPool[CurrentTotalPoolSize - 1].ID = 0;
	TweenPool[CurrentTotalPoolSize - 1].Handle = CurrentTotalPoolSize - 1;

	NextAvailableTween = 0;

	SET_DWORD_STAT(STAT_POOLED_TWEENS, CurrentTotalPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

void DueTweenPool::ExpandPool(const int& Amount)
{
	DECLARE_CYCLE_STAT(TEXT("ExpandPool"), STAT_ExpandPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ExpandPool);
	const int MaxPoolSize = GetDefault<UDueTweenSettings>()->MaxTweenPoolSize;
	if (CurrentTotalPoolSize == MaxPoolSize)
	{
		UE_LOG(LogDUETween, Warning,
		       TEXT("Tween Pool Has Already Reached Max Size: %d"), MaxPoolSize);
		return;
	}

	const int OldTweenPoolSize = CurrentTotalPoolSize;
	const FActiveDueTween* OldTweenPool = TweenPool;

	CurrentTotalPoolSize = FMath::Min(CurrentTotalPoolSize + Amount, MaxPoolSize);
	TweenPool = new FActiveDueTween[CurrentTotalPoolSize];

	// Copy old pool to new
	memcpy(TweenPool, OldTweenPool, sizeof(FActiveDueTween) * OldTweenPoolSize);

	TweenPool[OldTweenPoolSize - 1].TweenPtr.NextFreeTween = OldTweenPoolSize;
	for (int i = OldTweenPoolSize; i < CurrentTotalPoolSize - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = i + 1;
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[CurrentTotalPoolSize - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[CurrentTotalPoolSize - 1].Status = EDUETweenStatus::Unset;
	TweenPool[CurrentTotalPoolSize - 1].ID = 0;
	TweenPool[CurrentTotalPoolSize - 1].Handle = CurrentTotalPoolSize - 1;
	if (NextAvailableTween == NULL_DUETWEEN_HANDLE)
	{
		NextAvailableTween = OldTweenPoolSize;
	}
	INC_DWORD_STAT_BY(STAT_POOLED_TWEENS, CurrentTotalPoolSize - OldTweenPoolSize);
	SET_DWORD_STAT(STAT_TWEEN_POOL_SIZE, CurrentTotalPoolSize);
}

FActiveDueTween* DueTweenPool::GetTweenFromHandle(const FActiveDueTweenHandle TweenHandle) const
{
	if (TweenHandle == NULL_DUETWEEN_HANDLE || TweenHandle >= CurrentTotalPoolSize)
	{
		return nullptr;
	}
	return &TweenPool[TweenHandle];
}

FActiveDueTweenHandle DueTweenPool::GetTweenFromPool()
{
	DECLARE_CYCLE_STAT(TEXT("GetTweenFromPool"), STAT_GetTweenFromPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_GetTweenFromPool);

	const FActiveDueTweenHandle HandleFromPool = NextAvailableTween;
	if (NextAvailableTween == NULL_DUETWEEN_HANDLE)
	{
		UE_LOG(LogDUETween, Error,
		       TEXT("Unable to find available tween in pool"));
		return NULL_DUETWEEN_HANDLE;
	}
	UE_LOG(LogDUETween, Verbose, TEXT("Retrieved Tween: %u from pool"), GetTweenFromHandle(NextAvailableTween)->ID);

	NextAvailableTween = GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween;

	if (GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween == NULL_DUETWEEN_HANDLE)
	{
		// If there's only 1 tween left, expand the pool
		ExpandPool(GetDefault<UDueTweenSettings>()->PoolExpansionIncrement);
	}

	DEC_DWORD_STAT(STAT_POOLED_TWEENS);
	return HandleFromPool;
}

void DueTweenPool::ReturnTweenToPool(FActiveDueTweenHandle TweenToReturnHandle)
{
	FActiveDueTween* TweenToReturn = GetTweenFromHandle(TweenToReturnHandle);
	DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool"), STAT_ReturnTweenToPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool);
	UE_LOG(LogDUETween, Verbose, TEXT("Returning Tween: %u to pool"), TweenToReturn->ID);
	// add to free list
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_AddToFreeList"), STAT_ReturnTweenToPool_AddToFreeList,
		                   STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_AddToFreeList);
		TweenToReturn->Status = EDUETweenStatus::Unset;
		TweenToReturn->TweenPtr.NextFreeTween = NextAvailableTween;
		NextAvailableTween = TweenToReturn->Handle;

		INC_DWORD_STAT(STAT_POOLED_TWEENS);
	}
}

int DueTweenPool::GetCurrentPoolSize() const
{
	return CurrentTotalPoolSize;
}
