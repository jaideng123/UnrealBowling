#include "DUETween.h"

#include "DUEEasingFunctionLibrary.h"
#include "UObject/UnrealTypePrivate.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Pooled Tweens"), STAT_POOLED_TWEENS, STATGROUP_DUETWEEN);
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Active Tweens"), STAT_ACTIVE_TWEENS, STATGROUP_DUETWEEN);

void FDUETweenModule::InitTweenPool()
{
	if (TweenPool != nullptr)
	{
		delete[] TweenPool;
	}

	TWEEN_POOL_SIZE = INITIAL_POOL_SIZE;

	TweenPool = new FActiveDueTween[TWEEN_POOL_SIZE];

	for (int i = 0; i < TWEEN_POOL_SIZE - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = i + 1;
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[TWEEN_POOL_SIZE - 1].Status = EDUETweenStatus::Unset;
	TweenPool[TWEEN_POOL_SIZE - 1].ID = 0;
	TweenPool[TWEEN_POOL_SIZE - 1].Handle = TWEEN_POOL_SIZE - 1;

	NextAvailableTween = 0;

	SET_DWORD_STAT(STAT_POOLED_TWEENS, TWEEN_POOL_SIZE);

	ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;

	LastAssignedTweenId = 0;
}

void FDUETweenModule::ExpandPool(const int& Amount)
{
	DECLARE_CYCLE_STAT(TEXT("ExpandPool"), STAT_ExpandPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ExpandPool);

	if (TWEEN_POOL_SIZE == MAX_POOL_SIZE)
	{
		UE_LOG(LogDUETween, Warning,
		       TEXT("Tween Pool Has Already Reached Max Size: %d"), MAX_POOL_SIZE);
		return;
	}

	const int OldTweenPoolSize = TWEEN_POOL_SIZE;
	const FActiveDueTween* OldTweenPool = TweenPool;

	TWEEN_POOL_SIZE = FMath::Min(TWEEN_POOL_SIZE + Amount, MAX_POOL_SIZE);
	TweenPool = new FActiveDueTween[TWEEN_POOL_SIZE];

	// Copy old pool to new
	memcpy(TweenPool, OldTweenPool, sizeof(FActiveDueTween) * OldTweenPoolSize);

	TweenPool[OldTweenPoolSize - 1].TweenPtr.NextFreeTween = OldTweenPoolSize;
	for (int i = OldTweenPoolSize; i < TWEEN_POOL_SIZE - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = i + 1;
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = NULL_DUETWEEN_HANDLE;
	TweenPool[TWEEN_POOL_SIZE - 1].Status = EDUETweenStatus::Unset;
	TweenPool[TWEEN_POOL_SIZE - 1].ID = 0;
	TweenPool[TWEEN_POOL_SIZE - 1].Handle = TWEEN_POOL_SIZE - 1;
	if (NextAvailableTween == NULL_DUETWEEN_HANDLE)
	{
		NextAvailableTween = OldTweenPoolSize;
	}
}

FActiveDueTweenHandle FDUETweenModule::GetTweenFromPool()
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
		// If there's only 1 tween left, double the pool
		ExpandPool(TWEEN_POOL_SIZE);
	}

	DEC_DWORD_STAT(STAT_POOLED_TWEENS);
	return HandleFromPool;
}

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	FWorldDelegates::OnWorldBeginTearDown.AddRaw(this, &FDUETweenModule::HandleWorldBeginTearDown);
	InitTweenPool();
}

void FDUETweenModule::HandleWorldBeginTearDown(UWorld* World)
{
	UE_LOG(LogDUETween, Display, TEXT("Cleaning Up On World Teardown"));
	InitTweenPool();
}

void FDUETweenModule::ShutdownModule()
{
	UE_LOG(LogDUETween, Display, TEXT("UnLoaded DUETween"));
}

UWorld* FDUETweenModule::GetTickableGameObjectWorld() const
{
	return nullptr;
}

ETickableTickType FDUETweenModule::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

bool FDUETweenModule::IsTickable() const
{
	return ActiveTweenChainStart != NULL_DUETWEEN_HANDLE;
}

bool FDUETweenModule::IsTickableInEditor() const
{
	return false;
}

bool FDUETweenModule::IsTickableWhenPaused() const
{
	return false;
}

FValueContainer FDUETweenModule::GetCurrentValueFromProperty(const FDUETweenData& TweenData)
{
	if (!TweenData.Target.IsValid())
	{
		return FValueContainer();
	}
	switch (TweenData.ValueType)
	{
	case EDUEValueType::Float:
		{
			if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty))
			{
				float StartingValue;
				FloatProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Double:
		{
			if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty))
			{
				double StartingValue;
				DoubleProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Vector:
		{
			// We interpret null property as actor/component location
			if (TweenData.TargetProperty == nullptr)
			{
				if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					return FValueContainer(TargetAsSceneComponent->GetRelativeLocation());
				}
				if (const AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					return FValueContainer(TargetAsActor->GetActorLocation());
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
				return FValueContainer();
			}
			if (FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty); VectorProperty
				&& VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				const FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TweenData.Target.Get());
				FVector StartingValue = *StructAddress;

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Rotator:
		{
			// We interpret null property as actor/component rotation
			if (TweenData.TargetProperty == nullptr)
			{
				if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					return FValueContainer(TargetAsSceneComponent->GetRelativeRotation());
				}
				if (const AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					return FValueContainer(TargetAsActor->GetActorRotation());
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
				return FValueContainer();
			}
			if (const FStructProperty* RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				RotatorProperty
				&& RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				const FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(
					TweenData.Target.Get());
				FRotator StartingValue = *StructAddress;

				return FValueContainer(StartingValue);
			}
			break;
		}
	}
	UE_LOG(LogDUETween, Error,
	       TEXT("Unsupported Get for Value Type: %d"),
	       TweenData.ValueType);
	return FValueContainer();
}

void FDUETweenModule::SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue)
{
	DECLARE_CYCLE_STAT(TEXT("SetCurrentValue"), STAT_SetCurrentValue, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_SetCurrentValue);

	if (!TweenData.Target.IsValid())
	{
		return;
	}
	switch (TweenData.ValueType)
	{
	case EDUEValueType::Float:
		{
			if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty))
			{
				FloatProperty->SetValue_InContainer(TweenData.Target.Get(), NewValue.GetSubtype<float>());
			}
			break;
		}
	case EDUEValueType::Double:
		{
			if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty))
			{
				DoubleProperty->SetValue_InContainer(TweenData.Target.Get(), NewValue.GetSubtype<double>());
			}
			break;
		}
	case EDUEValueType::Vector:
		{
			const FVector NewVector = NewValue.GetSubtype<FVector>();
			UObject* TargetUObject = TweenData.Target.Get();
			// We interpret null property as actor/component location
			if (TweenData.TargetProperty == nullptr)
			{
				DECLARE_CYCLE_STAT(TEXT("SetCurrentValue_Cast"), STAT_SetCurrentValue_Cast, STATGROUP_DUETWEEN);
				SCOPE_CYCLE_COUNTER(STAT_SetCurrentValue_Cast);
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TargetUObject);
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeLocation(NewVector);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TargetUObject); TargetAsActor)
				{
					TargetAsActor->SetActorLocation(NewVector);
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null TargetUObject property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}

			if (const FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				VectorProperty
				&& VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TargetUObject);
				*StructAddress = NewVector;
			}
			break;
		}
	case EDUEValueType::Rotator:
		{
			FRotator NewRotatorValue = NewValue.GetSubtype<FRotator>();
			// We interpret null property as actor/component rotation
			if (TweenData.TargetProperty == nullptr)
			{
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeRotation(NewRotatorValue);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					TargetAsActor->SetActorRotation(NewRotatorValue);
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}
			if (const FStructProperty* RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				RotatorProperty
				&& RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(TweenData.Target.Get());
				*StructAddress = NewRotatorValue;
			}
			break;
		}
	default:
		{
			UE_LOG(LogDUETween, Error,
			       TEXT("Unsupported Set For Value Type: %d"),
			       TweenData.ValueType);
		}
	}
}

FActiveDueTweenHandle FDUETweenModule::AddTween(const FDUETweenData& TweenData)
{
	DECLARE_CYCLE_STAT(TEXT("AddTween"), STAT_AddTween, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_AddTween);
	FActiveDueTween* NewTweenObject = GetTweenFromHandle(GetTweenFromPool());
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
		NewTweenObject->TweenData = TweenData;
		NewTweenObject->Status = EDUETweenStatus::Running;
		NewTweenObject->TimeElapsed = 0;
		NewTweenObject->StartingValue = GetCurrentValueFromProperty(TweenData);
		NewTweenObject->ID = LastAssignedTweenId + 1;
		UE_LOG(LogDUETween, Verbose, TEXT("Creating Tween: %u"), NewTweenObject->ID);
		LastAssignedTweenId += 1;

		INC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}

	return NewTweenObject->Handle;
}

bool FDUETweenModule::PauseTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
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

bool FDUETweenModule::ResumeTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
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

bool FDUETweenModule::FastForwardTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
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

bool FDUETweenModule::StopTween(const FActiveDueTweenHandle& TweenHandle) const
{
	FActiveDueTween* Tween = GetTweenFromHandle(TweenHandle);
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

FActiveDueTween* FDUETweenModule::GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle) const
{
	if (TweenHandle == NULL_DUETWEEN_HANDLE || TweenHandle >= TWEEN_POOL_SIZE)
	{
		return nullptr;
	}
	return &TweenPool[TweenHandle];
}

FDUETweenModule& FDUETweenModule::Get()
{
	return FModuleManager::LoadModuleChecked<FDUETweenModule>("DUETween");
}

void FDUETweenModule::ReturnTweenToPool(const FActiveDueTweenHandle TweenToReturnHandle)
{
	FActiveDueTween* TweenToReturn = GetTweenFromHandle(TweenToReturnHandle);
	DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool"), STAT_ReturnTweenToPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool);
	UE_LOG(LogDUETween, Verbose, TEXT("Returning Tween: %u to pool"), TweenToReturn->ID);
	// First remove from the active tween chain
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_RemoveFromChain"), STAT_ReturnTweenToPool_RemoveFromChain,
		                   STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_RemoveFromChain);
		if (TweenToReturn->Handle == ActiveTweenChainStart)
		{
			ActiveTweenChainStart = GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.NextActiveTween;
		}
		else
		{
			FActiveDueTween* PreviousNode = GetTweenFromHandle(TweenToReturn->TweenPtr.ActiveNode.LastActiveTween);
			FActiveDueTween* NextNode = GetTweenFromHandle(TweenToReturn->TweenPtr.ActiveNode.NextActiveTween);
			if (PreviousNode != nullptr)
			{
				PreviousNode->TweenPtr.ActiveNode.NextActiveTween = TweenToReturn->TweenPtr.ActiveNode.NextActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u next now points to Tween: %u"), PreviousNode->ID,
				       NextNode != nullptr ?NextNode->ID : 0);
			}
			if (NextNode != nullptr)
			{
				NextNode->TweenPtr.ActiveNode.LastActiveTween = TweenToReturn->TweenPtr.ActiveNode.LastActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u previous now points to Tween: %u"), NextNode->ID,
				       PreviousNode != nullptr ? PreviousNode->ID: 0);
			}
		}
		DEC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
	// Then add to free list
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

void FDUETweenModule::TickTween(float DeltaTime, FActiveDueTween* CurrentTween)
{
	DECLARE_CYCLE_STAT(TEXT("TickTween"), STAT_TickTween, STATGROUP_DUETWEEN);
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

		DECLARE_CYCLE_STAT(TEXT("ProgressTween"), STAT_ProgressTween, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ProgressTween);

		switch (CurrentTween->TweenData.ValueType)
		{
		case EDUEValueType::Float:
			{
				const float NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<float>(),
				                                                      CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                      float>(),
				                                                      TweenProgress,
				                                                      CurrentTween->TweenData.EasingType,
				                                                      CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Float Value:: %f"), NewValue);
				SetCurrentValueToProperty(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDUEValueType::Double:
			{
				const double NewValue = DUEEasingFunctionLibrary::Ease(CurrentTween->StartingValue.GetSubtype<double>(),
				                                                       CurrentTween->TweenData.TargetValue.GetSubtype<
					                                                       double>(),
				                                                       TweenProgress,
				                                                       CurrentTween->TweenData.EasingType,
				                                                       CurrentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Double Value:: %f"), NewValue);
				SetCurrentValueToProperty(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDUEValueType::Vector:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
				                                                    1.0,
				                                                    TweenProgress,
				                                                    CurrentTween->TweenData.EasingType,
				                                                    CurrentTween->TweenData.Steps);
				const FVector NewValue = FMath::Lerp(CurrentTween->StartingValue.GetSubtype<FVector>(),
				                                     CurrentTween->TweenData.TargetValue.GetSubtype<FVector>(), Alpha);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector Value: %s"), *NewValue.ToString());
				SetCurrentValueToProperty(CurrentTween->TweenData, FValueContainer(NewValue));
				break;
			}
		case EDUEValueType::Rotator:
			{
				const double Alpha = DUEEasingFunctionLibrary::Ease(0,
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
				SetCurrentValueToProperty(CurrentTween->TweenData, FValueContainer(NewValue));
			}
			break;
		default:
			UE_LOG(LogDUETween, Error, TEXT("Unhandled Data type"));
			break;
		}
		if (TweenProgress >= 1.0)
		{
			CurrentTween->Status = EDUETweenStatus::Completed;
		}
	}
	else if (CurrentTween->Status == EDUETweenStatus::Paused)
	{
		// Do nothing
	}
	else if (CurrentTween->Status == EDUETweenStatus::Completed)
	{
		ReturnTweenToPool(CurrentTween->Handle);
	}
}


void FDUETweenModule::Tick(float deltaTime)
{
	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);

	FActiveDueTweenHandle CurrentTweenHandle = ActiveTweenChainStart;
	while (CurrentTweenHandle != NULL_DUETWEEN_HANDLE)
	{
		FActiveDueTween* currentTween = GetTweenFromHandle(CurrentTweenHandle);

		// Grab this before we tick in-case tween is removed from chain during tick
		FActiveDueTweenHandle nextTween = currentTween->TweenPtr.ActiveNode.NextActiveTween;

		TickTween(deltaTime, currentTween);

		CurrentTweenHandle = nextTween;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
