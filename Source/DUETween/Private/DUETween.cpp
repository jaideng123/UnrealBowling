#include "DUETween.h"

#include "DUEEasingFunctionLibrary.h"
#include "UObject/UnrealTypePrivate.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);

void FDUETweenModule::InitTweenPool()
{
	for (int i = 0; i < TWEEN_POOL_SIZE - 1; ++i)
	{
		TweenPool[i].TweenPtr.NextFreeTween = &TweenPool[i + 1];
		TweenPool[i].Status = EDUETweenStatus::Unset;
		TweenPool[i].ID = 0;
	}
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = nullptr;
	TweenPool[TWEEN_POOL_SIZE - 1].Status = EDUETweenStatus::Unset;
	TweenPool[TWEEN_POOL_SIZE - 1].ID = 0;
	NextAvailableTween = &TweenPool[0];

	ActiveTweenChainStart = nullptr;

	LastAssignedTweenId = 0;
	ActiveTweenCount = 0;
}

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	// TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	// TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 0.0f);
	FWorldDelegates::OnWorldBeginTearDown.AddRaw(this,&FDUETweenModule::HandleWorldBeginTearDown);
	InitTweenPool();
}

void FDUETweenModule::HandleWorldBeginTearDown(UWorld* World)
{
	UE_LOG(LogDUETween, Warning, TEXT("Cleaning Up On World Teardown"));
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

void FDUETweenModule::SetCurrentValueToProperty(const FDUETweenData& TweenData, FValueContainer newValue)
{
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
			// We interpret null property as location
			if (TweenData.TargetProperty == nullptr)
			{
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeLocation(newValue.GetSubtype<FVector>());
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					TargetAsActor->SetActorLocation(newValue.GetSubtype<FVector>());
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}

			auto VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (VectorProperty && VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TweenData.Target.Get());
				*StructAddress = newValue.GetSubtype<FVector>();
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

FActiveDueTween* FDUETweenModule::AddTween(const FDUETweenData& TweenData)
{
	// Retrieve Tween from pool
	auto tweenToSet = NextAvailableTween;
	if (tweenToSet == nullptr)
	{
		UE_LOG(LogDUETween, Error,
		       TEXT("Unable to find available tween in pool"));
		return nullptr;
	}
	UE_LOG(LogTemp, Display, TEXT("Retrieved Tween: %u from pool"), tweenToSet->ID);
	NextAvailableTween = NextAvailableTween->TweenPtr.NextFreeTween;

	// Add to tween chain
	tweenToSet->TweenPtr.ActiveNode.NextActiveTween = ActiveTweenChainStart;
	if (ActiveTweenChainStart != nullptr)
	{
		ActiveTweenChainStart->TweenPtr.ActiveNode.LastActiveTween = tweenToSet;
	}
	ActiveTweenChainStart = tweenToSet;

	tweenToSet->TweenData = TweenData;
	tweenToSet->Status = EDUETweenStatus::Running;
	tweenToSet->TimeElapsed = 0;
	tweenToSet->StartingValue = GetCurrentValueFromProperty(TweenData);
	tweenToSet->ID = LastAssignedTweenId + 1;
	UE_LOG(LogTemp, Display, TEXT("Creating Tween: %u"), tweenToSet->ID);
	LastAssignedTweenId += 1;

	ActiveTweenCount += 1;
	UE_LOG(LogTemp, Display, TEXT("%i Tweens Active"), ActiveTweenCount);

	return tweenToSet;
}

void FDUETweenModule::ReturnTweenToPool(FActiveDueTween* tween)
{
	UE_LOG(LogTemp, Display, TEXT("Returning Tween: %u to pool"), tween->ID);
	// First remove from the active tween chain
	if (tween == ActiveTweenChainStart)
	{
		ActiveTweenChainStart = ActiveTweenChainStart->TweenPtr.ActiveNode.NextActiveTween;
	}
	else
	{
		FActiveDueTween* previousNode = tween->TweenPtr.ActiveNode.LastActiveTween;
		FActiveDueTween* nextNode = tween->TweenPtr.ActiveNode.NextActiveTween;
		if (previousNode != nullptr)
		{
			previousNode->TweenPtr.ActiveNode.NextActiveTween = nextNode;
			UE_LOG(LogTemp, Display, TEXT("Tween: %u next now points to Tween: %u"), previousNode->ID,
			       nextNode != nullptr ?nextNode->ID : 0);
		}
		if (nextNode != nullptr)
		{
			nextNode->TweenPtr.ActiveNode.LastActiveTween = previousNode;
			UE_LOG(LogTemp, Display, TEXT("Tween: %u previous now points to Tween: %u"), nextNode->ID,
			       previousNode != nullptr ? previousNode->ID: 0);
		}
	}

	tween->Status = EDUETweenStatus::Unset;
	tween->TweenPtr.NextFreeTween = NextAvailableTween;
	NextAvailableTween = tween;

	ActiveTweenCount -= 1;
	UE_LOG(LogTemp, Display, TEXT("%i Tweens Active"), ActiveTweenCount);
}

void FDUETweenModule::TickTween(float deltaTime, FActiveDueTween* currentTween)
{
	if (currentTween->Status == EDUETweenStatus::Running)
	{
		currentTween->TimeElapsed += deltaTime;

		float progress = currentTween->TimeElapsed / currentTween->TweenData.Duration;

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
				// UE_LOG(LogTemp, Display, TEXT("Updating Float Value:: %f"), newValue);
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
				// UE_LOG(LogTemp, Display, TEXT("Updating Double Value:: %f"), newValue);
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
				// UE_LOG(LogTemp, Display, TEXT("Updating Vector Value: %s"), *newValue.ToString());
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

				// UE_LOG(LogTemp, Display, TEXT("Updating Rotator Value: %s"), *newValue.ToString());
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
		ReturnTweenToPool(currentTween);
	}
}

void FDUETweenModule::Tick(float deltaTime)
{
	int Tickcount = 0;

	double start = FPlatformTime::Seconds();

	if(USE_ARRAY)
	{
		for (int i = 0; i < TWEEN_POOL_SIZE; ++i)
		{
			Tickcount += 1;
			TickTween(deltaTime, &TweenPool[i]);
		}
	}
	else
	{
		FActiveDueTween* currentTween = ActiveTweenChainStart;
		while (currentTween != nullptr)
		{
			Tickcount += 1;
			FActiveDueTween* nextTween = currentTween->TweenPtr.ActiveNode.NextActiveTween;
			TickTween(deltaTime, currentTween);

			currentTween = nextTween;
		}
	}
	double end = FPlatformTime::Seconds();
	UE_LOG(LogDUETween, Display, TEXT("Tween Tick executed in %f ms."), (end-start) * 1000.0);
	UE_LOG(LogDUETween, Display, TEXT("Ticked %i Tweens"), Tickcount);
	return;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
