﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DUETweenSubsystem.h"

#include "DUETweenInternalUtils.h"
#include "Logging/StructuredLog.h"
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Active Tweens"), STAT_ACTIVE_TWEENS, STATGROUP_DUETween);


void UDUETweenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Pool.InitTweenPool();
	ActiveTweenChainStart = FActiveDUETweenHandle::NULL_HANDLE();
	LastAssignedTweenId = 0;
}

void UDUETweenSubsystem::Deinitialize()
{
	Pool.Deinitialize();
	Super::Deinitialize();
}

bool UDUETweenSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

FActiveDUETweenHandle UDUETweenSubsystem::AddTween(FDUETweenData& TweenData)
{
	DECLARE_CYCLE_STAT(TEXT("AddTween"), STAT_AddTween, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_AddTween);
	FActiveDUETween* NewTweenObject = GetTweenFromHandle(Pool.GetTweenFromPool(GetWorld()));
	ValidateTweenData(TweenData);
	if (NewTweenObject == nullptr)
	{
		return FActiveDUETweenHandle::NULL_HANDLE();
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_AddToChain"), STAT_AddTween_AddToChain, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_AddToChain);
		// Add to tween chain
		NewTweenObject->TweenPtr.ActiveNode.NextActiveTween = ActiveTweenChainStart;
		if (ActiveTweenChainStart != nullptr)
		{
			GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.LastActiveTween = NewTweenObject->Handle;
		}
		NewTweenObject->TweenPtr.ActiveNode.LastActiveTween = FActiveDUETweenHandle::NULL_HANDLE();
		ActiveTweenChainStart = NewTweenObject->Handle;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_Init"), STAT_AddTween_Init, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_Init);
		NewTweenObject->TweenData = MoveTemp(TweenData);
		NewTweenObject->Status = EDUETweenStatus::Running;
		NewTweenObject->TimeElapsed = 0;
		NewTweenObject->StartingValue = TweenData.StartingValue.GetCurrentSubtypeIndex() == static_cast<uint8>(-1)
			                                ? FDUETweenInternalUtils::GetCurrentValueFromProperty(TweenData)
			                                : TweenData.StartingValue;
		NewTweenObject->ID = LastAssignedTweenId + 1;
		UE_LOG(LogDUETween, Verbose, TEXT("Creating Tween: %u"), NewTweenObject->ID);
		LastAssignedTweenId += 1;

		INC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}

	return NewTweenObject->Handle;
}

bool UDUETweenSubsystem::PauseTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDUETweenStatus::Running)
	{
		Tween->Status = EDUETweenStatus::Paused;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::ResumeTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}

	if (Tween->Status == EDUETweenStatus::Paused)
	{
		Tween->Status = EDUETweenStatus::Running;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::FastForwardTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDUETweenStatus::Completed &&
		Tween->Status != EDUETweenStatus::FastForward)
	{
		Tween->Status = EDUETweenStatus::FastForward;
		return true;
	}
	return false;
}

bool UDUETweenSubsystem::StopTween(const FActiveDUETweenHandle& TweenHandle) const
{
	FActiveDUETween* Tween = GetTweenFromHandle(TweenHandle);
	if (Tween == nullptr || Tween->Status == EDUETweenStatus::Unset)
	{
		return false;
	}
	if (Tween->Status != EDUETweenStatus::Completed)
	{
		Tween->Status = EDUETweenStatus::Completed;
		return true;
	}
	return false;
}

void UDUETweenSubsystem::StopAllTweens(TWeakObjectPtr<UObject> Object) const
{
	if (!Object.IsValid())
	{
		return;
	}
	FActiveDUETweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	while (CurrentTweenHandle != nullptr)
	{
		FActiveDUETween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDUETweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;
		if (CurrentTween->TweenData.Target.IsValid() && CurrentTween->TweenData.Target.Get() == Object.Get())
		{
			StopTween(CurrentTweenHandle);
		}
		CurrentTweenHandle = NextTween;
	}
}

FActiveDUETween* UDUETweenSubsystem::GetTweenFromHandle(const FActiveDUETweenHandle& TweenHandle) const
{
	return Pool.GetTweenFromHandle(TweenHandle, true);
}

void UDUETweenSubsystem::Tick(float DeltaTime)
{
	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);
	FActiveDUETweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	while (CurrentTweenHandle != nullptr)
	{
		FActiveDUETween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		const FActiveDUETweenHandle NextTween = CurrentTween->TweenPtr.ActiveNode.NextActiveTween;

		TickTween(DeltaTime, CurrentTween);

		CurrentTweenHandle = NextTween;
	}
}

bool UDUETweenSubsystem::IsTickable() const
{
	return ActiveTweenChainStart != nullptr;
}

void UDUETweenSubsystem::TickTween(float DeltaTime, FActiveDUETween* CurrentTween)
{
	DECLARE_CYCLE_STAT(TEXT("TickTween"), STAT_TickTween, STATGROUP_DUETween);
	SCOPE_CYCLE_COUNTER(STAT_TickTween);


	// Early-Complete tween if target is gone
	if (!CurrentTween->TweenData.Target.IsValid() && CurrentTween->Status != EDUETweenStatus::Completed)
	{
		CurrentTween->Status = EDUETweenStatus::Completed;
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

	if (CurrentTween->Status == EDUETweenStatus::Running || CurrentTween->Status == EDUETweenStatus::FastForward)
	{
		CurrentTween->TimeElapsed += DeltaTime;

		float TweenProgress = CurrentTween->TimeElapsed / CurrentTween->TweenData.Duration;
		if (CurrentTween->Status == EDUETweenStatus::FastForward)
		{
			TweenProgress = 1.0;
		}

		DECLARE_CYCLE_STAT(TEXT("ProgressTween"), STAT_ProgressTween, STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_ProgressTween);

		FValueContainer UpdatedValue;
		switch (CurrentTween->TweenData.ValueType)
		{
		case EDueValueType::Float:
			{
				const float NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<float>(),
				                                                      CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                      float>(),
				                                                      TweenProgress,
				                                                      CurrentTween->TweenData.EasingType,
				                                                      CurrentTween->TweenData.Steps,
				                                                      CurrentTween->TweenData.ShouldYoYo);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Float Value:: %f"), NewValue);
				UpdatedValue.SetSubtype<float>(NewValue);
				break;
			}
		case EDueValueType::Double:
			{
				const double NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<double>(),
				                                                       CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                       double>(),
				                                                       TweenProgress,
				                                                       CurrentTween->TweenData.EasingType,
				                                                       CurrentTween->TweenData.Steps,
				                                                       CurrentTween->TweenData.ShouldYoYo);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Double Value:: %f"), NewValue);
				UpdatedValue.SetSubtype<double>(NewValue);
				break;
			}
		case EDueValueType::Vector:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps,
				                                                    CurrentTween->TweenData.ShouldYoYo);
				const FVector NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector>(),
				                                     CurrentTween->TweenData.TargetValue.GetSubtype<FVector>(), Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector Value: %s"), *NewValue.ToString());
				UpdatedValue.SetSubtype<FVector>(NewValue);
				break;
			}
		case EDueValueType::Rotator:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps,
				                                                    CurrentTween->TweenData.ShouldYoYo);
				const FRotator NewValue = FRotator(
					FQuat::Slerp(
						CurrentTween->StartingValue.GetSubtype<FRotator>().Quaternion(),
						CurrentTween->TweenData.TargetValue.GetSubtype<FRotator>().Quaternion(),
						Alpha)
				);

				UE_LOG(LogDUETween, Verbose, TEXT("Updating Rotator Value: %s"), *NewValue.ToString());
				UpdatedValue.SetSubtype<FRotator>(NewValue);
				break;
			}
		case EDueValueType::Vector2D:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps,
				                                                    CurrentTween->TweenData.ShouldYoYo);
				const FVector2D NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector2D>(),
				                                       CurrentTween->TweenData.TargetValue.GetSubtype<FVector2D>(),
				                                       Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector 2D Value: %s"), *NewValue.ToString());
				UpdatedValue.SetSubtype<FVector2D>(NewValue);
				break;
			}
		}
		if (UpdatedValue.GetCurrentSubtypeIndex() != static_cast<uint8>(-1))
		{
			FDUETweenInternalUtils::SetCurrentValue(CurrentTween->TweenData, UpdatedValue);
		}

		if (TweenProgress >= 1.0)
		{
			if (CurrentTween->TweenData.LoopCount != 0)
			{
				CurrentTween->TimeElapsed = 0;
				if (CurrentTween->TweenData.LoopCount > 0)
				{
					CurrentTween->TweenData.LoopCount -= 1;
				}
			}
			else
			{
				CurrentTween->Status = EDUETweenStatus::Completed;
			}
		}
	}
	else if (CurrentTween->Status == EDUETweenStatus::Paused)
	{
		// Do nothing
	}
	else if (CurrentTween->Status == EDUETweenStatus::Completed)
	{
		if(CurrentTween->TweenData.CompletionCallback)
		{
			CurrentTween->TweenData.CompletionCallback();
		}
		RemoveTweenFromActiveChain(CurrentTween->Handle);
		Pool.ReturnTweenToPool(CurrentTween->Handle);
	}
}

void UDUETweenSubsystem::ValidateTweenData(const FDUETweenData& TweenData)
{
#if !UE_BUILD_SHIPPING
	if (TweenData.TargetValue.GetCurrentSubtypeIndex() == static_cast<uint8>(-1))
	{
		UE_LOGFMT(LogDUETween, Error, "Attempting to create Tween on {0} with no target value set",
		          TweenData.Target.Get()->GetName());
	}
	if (TweenData.ValueType == EDueValueType::Unset)
	{
		UE_LOGFMT(LogDUETween, Error, "Attempting to create Tween on {0} with no value type set",
		          TweenData.Target.Get()->GetName());
	}
	if (TweenData.UpdateType == EDueUpdateType::Unset)
	{
		UE_LOGFMT(LogDUETween, Error, "Attempting to create Tween on {0} with no update type set",
		          TweenData.Target.Get()->GetName());
	}
	// Currently we only check for other properties
	if (TweenData.TargetProperty != nullptr)
	{
		FActiveDUETweenHandle CurrentTweenHandle = ActiveTweenChainStart;
		while (CurrentTweenHandle != nullptr)
		{
			FActiveDUETween* CurrentTween = GetTweenFromHandle(CurrentTweenHandle);
			if (CurrentTween->TweenData.Target != nullptr &&
				CurrentTween->TweenData.TargetProperty == TweenData.TargetProperty &&
				CurrentTween->Status == EDUETweenStatus::Running &&
				CurrentTween->TweenData.Target.HasSameIndexAndSerialNumber(TweenData.Target.Get()))
			{
				UE_LOGFMT(LogDUETween, Error, "Creating Tween on {0} that is already actively tweening property {1}",
				          TweenData.Target.Get()->GetName(), TweenData.TargetProperty->GetName());
			}

			CurrentTweenHandle = CurrentTween->TweenPtr.NextFreeTween;
		}
	}
#endif
}

void UDUETweenSubsystem::RemoveTweenFromActiveChain(FActiveDUETweenHandle TweenHandleToRemove)
{
	const FActiveDUETween* TweenToRemove = Pool.GetTweenFromHandle(TweenHandleToRemove, true);
	if(TweenToRemove == nullptr)
	{
		return;
	}
	// First remove from the active tween chain
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_RemoveFromChain"), STAT_ReturnTweenToPool_RemoveFromChain,
		                   STATGROUP_DUETween);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_RemoveFromChain);
		if (TweenToRemove->Handle == ActiveTweenChainStart)
		{
			ActiveTweenChainStart = Pool.GetTweenFromHandle(ActiveTweenChainStart, true)->TweenPtr.ActiveNode.NextActiveTween;
		}
		else
		{
			FActiveDUETweenHandle PreviousNodeHandle = TweenToRemove->TweenPtr.ActiveNode.LastActiveTween;
			FActiveDUETweenHandle NextNodeHandle = TweenToRemove->TweenPtr.ActiveNode.NextActiveTween;
			if (PreviousNodeHandle != nullptr)
			{
			    FActiveDUETween* PreviousNode = Pool.GetTweenFromHandle(PreviousNodeHandle, true);
				PreviousNode->TweenPtr.ActiveNode.NextActiveTween = TweenToRemove->TweenPtr.ActiveNode.NextActiveTween;
			}
			if (NextNodeHandle != nullptr)
			{
            	FActiveDUETween* NextNode = Pool.GetTweenFromHandle(NextNodeHandle, true);
				NextNode->TweenPtr.ActiveNode.LastActiveTween = TweenToRemove->TweenPtr.ActiveNode.LastActiveTween;
			}
		}
		DEC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
}
