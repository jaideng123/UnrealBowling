// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenSubsystem.h"

void UDueTweenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// TODO: this should never get called twice, need to make sure these each have their own pools
	FDUETweenModule::Get().InitTweenPool();
}

void UDueTweenSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDueTweenSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

void UDueTweenSubsystem::Tick(float DeltaTime)
{
	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);
	FActiveDueTweenHandle CurrentTweenHandle = FDUETweenModule::Get().GetActiveTweenChainStart();
	TSet<int> VisitedHandles;
	while (CurrentTweenHandle != NULL_DUETWEEN_HANDLE)
	{
		FActiveDueTween* CurrentTween = FDUETweenModule::Get().GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDueTweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;

		FDUETweenModule::Get().TickTween(DeltaTime, CurrentTween);

		CurrentTweenHandle = NextTween;
	}
}

bool UDueTweenSubsystem::IsTickable() const
{
	return FDUETweenModule::Get().GetActiveTweenChainStart() != NULL_DUETWEEN_HANDLE;
}
