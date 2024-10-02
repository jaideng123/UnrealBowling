// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenSubsystem.h"

#include "DueTweenInternalUtils.h"
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Active Tweens"), STAT_ACTIVE_TWEENS, STATGROUP_DUETWEEN);


void UDueTweenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Pool.InitTweenPool();
	ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;
	LastAssignedTweenId = 0;
}

void UDueTweenSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDueTweenSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

FActiveDueTweenHandle UDueTweenSubsystem::AddTween(FDUETweenData& TweenData)
{
	DECLARE_CYCLE_STAT(TEXT("AddTween"), STAT_AddTween, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_AddTween);
	FActiveDueTween* NewTweenObject = GetTweenFromHandle(Pool.GetTweenFromPool());
	if (NewTweenObject == nullptr)
	{
		return NULL_DUETWEEN_HANDLE;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_AddToChain"), STAT_AddTween_AddToChain, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_AddToChain);
		// Add to tween chain
		NewTweenObject->TweenPtr.ActiveNode.NextActiveTween = ActiveTweenChainStart;
		if (ActiveTweenChainStart != NULL_DUETWEEN_HANDLE)
		{
			GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.LastActiveTween = NewTweenObject->Handle;
		}
		NewTweenObject->TweenPtr.ActiveNode.LastActiveTween = NULL_DUETWEEN_HANDLE;
		ActiveTweenChainStart = NewTweenObject->Handle;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_Init"), STAT_AddTween_Init, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_Init);
		NewTweenObject->TweenData = MoveTemp(TweenData);
		NewTweenObject->Status = EDueTweenStatus::Running;
		NewTweenObject->TimeElapsed = 0;
		NewTweenObject->StartingValue = TweenData.StartingValue.GetCurrentSubtypeIndex() == static_cast<uint8>(-1)
			                                ? FDueTweenInternalUtils::GetCurrentValueFromProperty(TweenData)
			                                : TweenData.StartingValue;
		NewTweenObject->ID = LastAssignedTweenId + 1;
		UE_LOG(LogDUETween, Verbose, TEXT("Creating Tween: %u"), NewTweenObject->ID);
		LastAssignedTweenId += 1;

		INC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}

	return NewTweenObject->Handle;
}

bool UDueTweenSubsystem::PauseTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDueTweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDueTweenStatus::Running)
	{
		Tween->Status = EDueTweenStatus::Paused;
		return true;
	}
	return false;
}

bool UDueTweenSubsystem::ResumeTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDueTweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDueTweenStatus::Paused)
	{
		Tween->Status = EDueTweenStatus::Running;
		return true;
	}
	return false;
}

bool UDueTweenSubsystem::FastForwardTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDueTweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDueTweenStatus::Completed &&
		Tween->Status != EDueTweenStatus::FastForward)
	{
		Tween->Status = EDueTweenStatus::FastForward;
		return true;
	}
	return false;
}

bool UDueTweenSubsystem::StopTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDueTweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDueTweenStatus::Completed)
	{
		Tween->Status = EDueTweenStatus::Completed;
		return true;
	}
	return false;
}

FActiveDueTween* UDueTweenSubsystem::GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle) const
{
	return Pool.GetTweenFromHandle(TweenHandle);
}

void UDueTweenSubsystem::Tick(float DeltaTime)
{
	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);
	FActiveDueTweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	TSet<int> VisitedHandles;
	while (CurrentTweenHandle != NULL_DUETWEEN_HANDLE)
	{
		FActiveDueTween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDueTweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;

		TickTween(DeltaTime, CurrentTween);

		CurrentTweenHandle = NextTween;
	}
}

bool UDueTweenSubsystem::IsTickable() const
{
	return ActiveTweenChainStart != NULL_DUETWEEN_HANDLE;
}

void UDueTweenSubsystem::TickTween(float DeltaTime, FActiveDueTween* CurrentTween)
{
	DECLARE_CYCLE_STAT(TEXT("TickTween"), STAT_TickTween, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickTween);


	// Early-Complete tween if target is gone
	if (!CurrentTween->TweenData.Target.IsValid() && CurrentTween->Status != EDueTweenStatus::Completed)
	{
		CurrentTween->Status = EDueTweenStatus::Completed;
		return;
	}

	// Make sure we're not paused
	if (CurrentTween->TweenData.Target.IsValid())
	{
		UWorld* TweenWorld = CurrentTween->TweenData.Target.Get()->GetWorld();
		if (TweenWorld && TweenWorld->IsPaused())
		{
#if WITH_EDITOR
			if (!TweenWorld->bDebugFrameStepExecutedThisFrame)
			{
#endif
				UE_LOG(LogDUETween, Verbose, TEXT("Skipping Paused Tween"));
				return;
#if WITH_EDITOR
			}
#endif
		}
	}

	if (CurrentTween->Status == EDueTweenStatus::Running || CurrentTween->Status == EDueTweenStatus::FastForward)
	{
		CurrentTween->TimeElapsed += DeltaTime;

		float TweenProgress = CurrentTween->TimeElapsed / CurrentTween->TweenData.Duration;
		if (CurrentTween->Status == EDueTweenStatus::FastForward)
		{
			TweenProgress = 1.0;
		}

		DECLARE_CYCLE_STAT(TEXT("ProgressTween"), STAT_ProgressTween, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ProgressTween);

		switch (CurrentTween->TweenData.ValueType)
		{
		case EDueValueType::Float:
			{
				const float NewValue = DueEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<float>(),
				                                                      CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                      float>(),
				                                                      TweenProgress,
				                                                      CurrentTween->TweenData.EasingType,
				                                                      CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Float Value:: %f"), NewValue);
				FDueTweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Double:
			{
				const double NewValue = DueEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<double>(),
				                                                       CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                       double>(),
				                                                       TweenProgress,
				                                                       CurrentTween->TweenData.EasingType,
				                                                       CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Double Value:: %f"), NewValue);
				FDueTweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Vector:
			{
				const double Alpha = DueEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FVector NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector>(),
				                                     CurrentTween->TweenData.TargetValue.GetSubtype<FVector>(), Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector Value: %s"), *NewValue.ToString());
				FDueTweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDueValueType::Rotator:
			{
				const double Alpha = DueEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FRotator NewValue = FRotator(
					FQuat::Slerp(
						CurrentTween->StartingValue.GetSubtype<FRotator>().Quaternion(),
						CurrentTween->TweenData.TargetValue.GetSubtype<FRotator>().Quaternion(),
						Alpha)
				);

				UE_LOG(LogDUETween, Verbose, TEXT("Updating Rotator Value: %s"), *NewValue.ToString());
				FDueTweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
			}
			break;
		case EDueValueType::Vector2D:
			{
				const double Alpha = DueEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FVector2D NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector2D>(),
				                                       CurrentTween->TweenData.TargetValue.GetSubtype<FVector2D>(),
				                                       Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector 2D Value: %s"), *NewValue.ToString());
				FDueTweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
			break;
		}
		if (TweenProgress >= 1.0)
		{
			CurrentTween->Status = EDueTweenStatus::Completed;
		}
	}
	else if (CurrentTween->Status == EDueTweenStatus::Paused)
	{
		// Do nothing
	}
	else if (CurrentTween->Status == EDueTweenStatus::Completed)
	{
		RemoveTweenFromActiveChain(CurrentTween->Handle);
		Pool.ReturnTweenToPool(CurrentTween->Handle);
	}
}

void UDueTweenSubsystem::RemoveTweenFromActiveChain(FActiveDueTweenHandle TweenHandleToRemove)
{
	const FActiveDueTween* TweenToRemove = Pool.GetTweenFromHandle(TweenHandleToRemove);
	// First remove from the active tween chain
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_RemoveFromChain"), STAT_ReturnTweenToPool_RemoveFromChain,
		                   STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_RemoveFromChain);
		if (TweenToRemove->Handle == ActiveTweenChainStart)
		{
			ActiveTweenChainStart = Pool.GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.NextActiveTween;
		}
		else
		{
			FActiveDueTween* PreviousNode = Pool.GetTweenFromHandle(TweenToRemove->TweenPtr.ActiveNode.LastActiveTween);
			FActiveDueTween* NextNode = Pool.GetTweenFromHandle(TweenToRemove->TweenPtr.ActiveNode.NextActiveTween);
			if (PreviousNode != nullptr)
			{
				PreviousNode->TweenPtr.ActiveNode.NextActiveTween = TweenToRemove->TweenPtr.ActiveNode.NextActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u next now points to Tween: %u"), PreviousNode->ID,
				       NextNode != nullptr ?NextNode->ID : 0);
			}
			if (NextNode != nullptr)
			{
				NextNode->TweenPtr.ActiveNode.LastActiveTween = TweenToRemove->TweenPtr.ActiveNode.LastActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u previous now points to Tween: %u"), NextNode->ID,
				       PreviousNode != nullptr ? PreviousNode->ID: 0);
			}
		}
		DEC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
}
