#include "DUETween.h"

#include "DUEEasingFunctionLibrary.h"
#include "UObject/UnrealTypePrivate.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Pooled Tweens"), STAT_POOLED_TWEENS, STATGROUP_DUETWEEN);
void FDUETweenModule::InitTweenPool()
{
	if(TweenPool != nullptr)
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
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = INVALID_DUETWEEN_HANDLE;
	TweenPool[TWEEN_POOL_SIZE - 1].Status = EDUETweenStatus::Unset;
	TweenPool[TWEEN_POOL_SIZE - 1].ID = 0;
	TweenPool[TWEEN_POOL_SIZE - 1].Handle = TWEEN_POOL_SIZE - 1;
	
	NextAvailableTween = 0;

	SET_DWORD_STAT(STAT_POOLED_TWEENS, TWEEN_POOL_SIZE);

	ActiveTweenChainStart = INVALID_DUETWEEN_HANDLE;

	LastAssignedTweenId = 0;
}

void FDUETweenModule::ExpandPool(int Amount)
{
	DECLARE_CYCLE_STAT(TEXT("ExpandPool"), STAT_ExpandPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ExpandPool);
	
	int oldTweenPoolSize = TWEEN_POOL_SIZE;
	auto OldTweenPool = TweenPool;
	
	TWEEN_POOL_SIZE = TWEEN_POOL_SIZE + Amount;
	TweenPool = new FActiveDueTween[TWEEN_POOL_SIZE];

	// Copy old pool to new
	memcpy(TweenPool, OldTweenPool, sizeof(FActiveDueTween) * oldTweenPoolSize);

	TweenPool[oldTweenPoolSize - 1].TweenPtr.NextFreeTween = oldTweenPoolSize;
	for (int i = oldTweenPoolSize; i < TWEEN_POOL_SIZE-1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = i + 1;
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
		TweenPool[i].Handle = i;
	}
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = INVALID_DUETWEEN_HANDLE;
	TweenPool[TWEEN_POOL_SIZE - 1].Status = EDUETweenStatus::Unset;
	TweenPool[TWEEN_POOL_SIZE - 1].ID = 0;
	TweenPool[TWEEN_POOL_SIZE - 1].Handle = TWEEN_POOL_SIZE - 1;
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
			auto FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty);
			if (FloatProperty)
			{
				float StartingValue;
				FloatProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Double:
		{
			auto DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty);
			if (DoubleProperty)
			{
				double StartingValue;
				DoubleProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Vector:
		{
			// We interpret null property as location
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
			auto VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (VectorProperty && VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TweenData.Target.Get());
				FVector StartingValue = *StructAddress;

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDUEValueType::Rotator:
		{
			// We interpret null property as rotation
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
			auto RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (RotatorProperty && RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(TweenData.Target.Get());
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

void FDUETweenModule::SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& newValue)
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
			auto FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty);
			if (FloatProperty)
			{
				FloatProperty->SetValue_InContainer(TweenData.Target.Get(), newValue.GetSubtype<float>());
			}
			break;
		}
	case EDUEValueType::Double:
		{
			auto DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty);
			if (DoubleProperty)
			{
				DoubleProperty->SetValue_InContainer(TweenData.Target.Get(), newValue.GetSubtype<double>());
			}
			break;
		}
	case EDUEValueType::Vector:
		{
			FVector newVector = newValue.GetSubtype<FVector>();
			UObject* target = TweenData.Target.Get();
			// We interpret null property as location
			if (TweenData.TargetProperty == nullptr)
			{
				DECLARE_CYCLE_STAT(TEXT("SetCurrentValue_Cast"), STAT_SetCurrentValue_Cast, STATGROUP_DUETWEEN);
				SCOPE_CYCLE_COUNTER(STAT_SetCurrentValue_Cast);
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(target);
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeLocation(newVector);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(target); TargetAsActor)
				{
					TargetAsActor->SetActorLocation(newVector);
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}

			auto VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (VectorProperty && VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(target);
				*StructAddress = newVector;
			}
			break;
		}
	case EDUEValueType::Rotator:
		{
			// We interpret null property as rotation
			if (TweenData.TargetProperty == nullptr)
			{
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					FRotator rotator = newValue.GetSubtype<FRotator>();
					TargetAsSceneComponent->SetRelativeRotation(rotator);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					TargetAsActor->SetActorRotation(newValue.GetSubtype<FRotator>());
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}
			auto RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (RotatorProperty && RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(TweenData.Target.Get());
				*StructAddress = newValue.GetSubtype<FRotator>();
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

DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Active Tweens"), STAT_ACTIVE_TWEENS, STATGROUP_DUETWEEN);
FActiveDueTweenHandle FDUETweenModule::AddTween(const FDUETweenData& TweenData)
{
	DECLARE_CYCLE_STAT(TEXT("AddTween"), STAT_AddTween, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_AddTween);
	FActiveDueTween* tweenToSet = nullptr;
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_Retrieve"), STAT_AddTween_Retrieve, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_Retrieve);
		// Retrieve Tween from pool
		tweenToSet = GetTweenFromHandle(NextAvailableTween);
		if (tweenToSet == nullptr)
		{
			UE_LOG(LogDUETween, Error,
			       TEXT("Unable to find available tween in pool"));
			return INVALID_DUETWEEN_HANDLE;
		}
		UE_LOG(LogDUETween, Verbose, TEXT("Retrieved Tween: %u from pool"), tweenToSet->ID);
		NextAvailableTween = GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween;
		DEC_DWORD_STAT(STAT_POOLED_TWEENS);
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_AddToChain"), STAT_AddTween_AddToChain, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_AddToChain);
		// Add to tween chain
		tweenToSet->TweenPtr.ActiveNode.NextActiveTween = ActiveTweenChainStart;
		if (ActiveTweenChainStart != INVALID_DUETWEEN_HANDLE)
		{
			GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.LastActiveTween = tweenToSet->Handle;
		}
		ActiveTweenChainStart = tweenToSet->Handle;
	}
	{
		DECLARE_CYCLE_STAT(TEXT("AddTween_Init"), STAT_AddTween_Init, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_AddTween_Init);
		tweenToSet->TweenData = TweenData;
		tweenToSet->Status = EDUETweenStatus::Running;
		tweenToSet->TimeElapsed = 0;
		tweenToSet->StartingValue = GetCurrentValueFromProperty(TweenData);
		tweenToSet->ID = LastAssignedTweenId + 1;
		UE_LOG(LogDUETween, Verbose, TEXT("Creating Tween: %u"), tweenToSet->ID);
		LastAssignedTweenId += 1;

		INC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
	
	// Copy handle before we possibly expand the pool
	FActiveDueTweenHandle returnHandle = tweenToSet->Handle;
	
	if(GetTweenFromHandle(NextAvailableTween)->TweenPtr.NextFreeTween == INVALID_DUETWEEN_HANDLE)
	{
		ExpandPool(TWEEN_POOL_SIZE / 2);
	}

	return returnHandle;
}

FActiveDueTween* FDUETweenModule::GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle)
{
	if(TweenHandle == INVALID_DUETWEEN_HANDLE)
	{
		return nullptr;
	}
	return &TweenPool[TweenHandle];
}

void FDUETweenModule::ReturnTweenToPool(FActiveDueTweenHandle tweenHandle)
{
	FActiveDueTween* tween = GetTweenFromHandle(tweenHandle);
	DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool"), STAT_ReturnTweenToPool, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool);
	UE_LOG(LogDUETween, Verbose, TEXT("Returning Tween: %u to pool"), tween->ID);
	// First remove from the active tween chain
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_RemoveFromChain"), STAT_ReturnTweenToPool_RemoveFromChain, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_RemoveFromChain);
		if (tween->Handle == ActiveTweenChainStart)
		{
			ActiveTweenChainStart = GetTweenFromHandle(ActiveTweenChainStart)->TweenPtr.ActiveNode.NextActiveTween;
		}
		else
		{
			FActiveDueTween* previousNode = GetTweenFromHandle(tween->TweenPtr.ActiveNode.LastActiveTween);
			FActiveDueTween* nextNode = GetTweenFromHandle(tween->TweenPtr.ActiveNode.NextActiveTween);
			if (previousNode != nullptr)
			{
				previousNode->TweenPtr.ActiveNode.NextActiveTween = tween->TweenPtr.ActiveNode.NextActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u next now points to Tween: %u"), previousNode->ID,
				       nextNode != nullptr ?nextNode->ID : 0);
			}
			if (nextNode != nullptr)
			{
				nextNode->TweenPtr.ActiveNode.LastActiveTween = tween->TweenPtr.ActiveNode.LastActiveTween;
				UE_LOG(LogDUETween, Verbose, TEXT("Tween: %u previous now points to Tween: %u"), nextNode->ID,
				       previousNode != nullptr ? previousNode->ID: 0);
			}
		}
		DEC_DWORD_STAT(STAT_ACTIVE_TWEENS);
	}
	// Then add to free list
	{
		DECLARE_CYCLE_STAT(TEXT("ReturnTweenToPool_AddToFreeList"), STAT_ReturnTweenToPool_AddToFreeList, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ReturnTweenToPool_AddToFreeList);
		tween->Status = EDUETweenStatus::Unset;
		tween->TweenPtr.NextFreeTween = NextAvailableTween;
		NextAvailableTween = tween->Handle;

		INC_DWORD_STAT(STAT_POOLED_TWEENS);
	}
}

void FDUETweenModule::TickTween(float deltaTime, FActiveDueTween* currentTween)
{
	DECLARE_CYCLE_STAT(TEXT("TickTween"), STAT_TickTween, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickTween);
	if (currentTween->Status == EDUETweenStatus::Running)
	{
		// Early-Complete tweens if target is gone
		if(!currentTween->TweenData.Target.IsValid())
		{
			currentTween->Status = EDUETweenStatus::Completed;
			return;
		}
		
		currentTween->TimeElapsed += deltaTime;

		float progress = currentTween->TimeElapsed / currentTween->TweenData.Duration;

		DECLARE_CYCLE_STAT(TEXT("ProgressTween"), STAT_ProgressTween, STATGROUP_DUETWEEN);
		SCOPE_CYCLE_COUNTER(STAT_ProgressTween);

		switch (currentTween->TweenData.ValueType)
		{
		case EDUEValueType::Float:
			{
				float newValue = DUEEasingFunctionLibrary::Ease(currentTween->StartingValue.GetSubtype<float>(),
				                                                currentTween->TweenData.TargetValue.GetSubtype<
					                                                float>(),
				                                                progress,
				                                                currentTween->TweenData.EasingType,
				                                                currentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Float Value:: %f"), newValue);
				SetCurrentValueToProperty(currentTween->TweenData, FValueContainer(newValue));
				break;
			}
		case EDUEValueType::Double:
			{
				double newValue = DUEEasingFunctionLibrary::Ease(currentTween->StartingValue.GetSubtype<double>(),
				                                                 currentTween->TweenData.TargetValue.GetSubtype<
					                                                 double>(),
				                                                 progress,
				                                                 currentTween->TweenData.EasingType,
				                                                 currentTween->TweenData.Steps);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Double Value:: %f"), newValue);
				SetCurrentValueToProperty(currentTween->TweenData, FValueContainer(newValue));
				break;
			}
		case EDUEValueType::Vector:
			{
				double ease = DUEEasingFunctionLibrary::Ease(0,
				                                             1.0,
				                                             progress,
				                                             currentTween->TweenData.EasingType,
				                                             currentTween->TweenData.Steps);
				FVector newValue = FMath::Lerp(currentTween->StartingValue.GetSubtype<FVector>(),
				                               currentTween->TweenData.TargetValue.GetSubtype<FVector>(), ease);
				UE_LOG(LogDUETween, Verbose, TEXT("Updating Vector Value: %s"), *newValue.ToString());
				SetCurrentValueToProperty(currentTween->TweenData, FValueContainer(newValue));
				break;
			}
		case EDUEValueType::Rotator:
			{
				double ease = DUEEasingFunctionLibrary::Ease(0,
				                                             1.0,
				                                             progress,
				                                             currentTween->TweenData.EasingType,
				                                             currentTween->TweenData.Steps);
				FRotator newValue = FRotator(FQuat::Slerp(
					currentTween->StartingValue.GetSubtype<FRotator>().Quaternion(),
					currentTween->TweenData.TargetValue.GetSubtype<FRotator>().
					              Quaternion(), ease));

				UE_LOG(LogDUETween, Verbose, TEXT("Updating Rotator Value: %s"), *newValue.ToString());
				SetCurrentValueToProperty(currentTween->TweenData, FValueContainer(newValue));
			}
			break;
		default:
			UE_LOG(LogDUETween, Error, TEXT("Unhandled Data type"));
			break;
		}
		if (progress >= 1.0)
		{
			currentTween->Status = EDUETweenStatus::Completed;
		}
	}
	else if (currentTween->Status == EDUETweenStatus::Completed)
	{
		ReturnTweenToPool(currentTween->Handle);
	}
}


void FDUETweenModule::Tick(float deltaTime)
{
	int Tickcount = 0;

	DECLARE_CYCLE_STAT(TEXT("TickAllTweens"), STAT_TickAllTweens, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_TickAllTweens);

	// TODO: clean this up after perf testing
	static const bool USE_ARRAY = false;
	if (USE_ARRAY)
	{
		for (int i = 0; i < TWEEN_POOL_SIZE; ++i)
		{
			Tickcount += 1;
			TickTween(deltaTime, &TweenPool[i]);
		}
	}
	else
	{
		FActiveDueTweenHandle CurrentTweenHandle = ActiveTweenChainStart;
		while (CurrentTweenHandle != INVALID_DUETWEEN_HANDLE)
		{
			FActiveDueTween* currentTween = GetTweenFromHandle(CurrentTweenHandle);
			Tickcount += 1;
			// Grab this before we tick in-case tween is removed from chain
			FActiveDueTweenHandle nextTween = currentTween->TweenPtr.ActiveNode.NextActiveTween;
			TickTween(deltaTime, currentTween);

			CurrentTweenHandle = nextTween;
		}
	}
	UE_LOG(LogDUETween, Verbose, TEXT("Ticked %i Tweens"), Tickcount);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
