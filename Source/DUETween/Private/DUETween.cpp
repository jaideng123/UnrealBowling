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
	}
	TweenPool[TWEEN_POOL_SIZE - 1].TweenPtr.NextFreeTween = nullptr;
	NextAvailableTween = &TweenPool[0];
}

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 0.0f);
	// Initialize Free List
	InitTweenPool();
}

void FDUETweenModule::ShutdownModule()
{
	UE_LOG(LogDUETween, Display, TEXT("UnLoaded DUETween"));

	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
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
	NextAvailableTween = NextAvailableTween->TweenPtr.NextFreeTween;
	if (tweenToSet == nullptr)
	{
		UE_LOG(LogDUETween, Error,
		       TEXT("Unable to find available tween in pool"));
		return nullptr;
	}

	// Add to tween chain
	tweenToSet->TweenPtr.NextActiveTween = ActiveTweenChainStart;
	ActiveTweenChainStart = tweenToSet;


	tweenToSet->TweenData = TweenData;
	tweenToSet->Status = EDUETweenStatus::Running;
	tweenToSet->TimeElapsed = 0;
	tweenToSet->StartingValue = GetCurrentValueFromProperty(TweenData);
	return tweenToSet;
}

void FDUETweenModule::ReturnTweenToPool(FActiveDueTween* tween)
{
	tween->Status = EDUETweenStatus::Unset;
	tween->TweenPtr.NextFreeTween = NextAvailableTween;
	NextAvailableTween = tween;
}

bool FDUETweenModule::Tick(float deltaTime)
{
	// Only Run Tweens During Play
	if (GEngine->GetCurrentPlayWorld())
	{
		return true;
	}


	for (FActiveDueTween* tween = ActiveTweenChainStart; tween != nullptr; tween = tween->
	     TweenPtr.NextActiveTween)
	{
		if (tween->Status == EDUETweenStatus::Running)
		{
			tween->TimeElapsed += deltaTime;

			float progress = tween->TimeElapsed / tween->TweenData.Duration;

			switch (tween->TweenData.ValueType)
			{
			case EDUEValueType::Float:
				{
					float newValue = DUEEasingFunctionLibrary::Ease(tween->StartingValue.GetSubtype<float>(),
					                                                tween->TweenData.TargetValue.GetSubtype<
						                                                float>(),
					                                                progress,
					                                                tween->TweenData.EasingType,
					                                                tween->TweenData.Steps);
					UE_LOG(LogTemp, Display, TEXT("Updating Float Value:: %f"), newValue);
					SetCurrentValueToProperty(tween->TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Double:
				{
					double newValue = DUEEasingFunctionLibrary::Ease(tween->StartingValue.GetSubtype<double>(),
					                                                 tween->TweenData.TargetValue.GetSubtype<double>(),
					                                                 progress,
					                                                 tween->TweenData.EasingType,
					                                                 tween->TweenData.Steps);
					UE_LOG(LogTemp, Display, TEXT("Updating Double Value:: %f"), newValue);
					SetCurrentValueToProperty(tween->TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Vector:
				{
					double ease = DUEEasingFunctionLibrary::Ease(0,
					                                             1.0,
					                                             progress,
					                                             tween->TweenData.EasingType,
					                                             tween->TweenData.Steps);
					FVector newValue = FMath::Lerp(tween->StartingValue.GetSubtype<FVector>(),
					                               tween->TweenData.TargetValue.GetSubtype<FVector>(), ease);
					UE_LOG(LogTemp, Display, TEXT("Updating Vector Value: %s"), *newValue.ToString());
					SetCurrentValueToProperty(tween->TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Rotator:
				{
					double ease = DUEEasingFunctionLibrary::Ease(0,
					                                             1.0,
					                                             progress,
					                                             tween->TweenData.EasingType,
					                                             tween->TweenData.Steps);
					FRotator newValue = FRotator(FQuat::Slerp(tween->StartingValue.GetSubtype<FRotator>().Quaternion(),
					                                          tween->TweenData.TargetValue.GetSubtype<FRotator>().
					                                                 Quaternion(), ease));

					UE_LOG(LogTemp, Display, TEXT("Updating Rotator Value: %s"), *newValue.ToString());
					SetCurrentValueToProperty(tween->TweenData, FValueContainer(newValue));
				}
				break;
			default:
				UE_LOG(LogDUETween, Error, TEXT("Unhandled Data type"));
				break;
			}
			if (progress >= 1.0)
			{
				tween->Status = EDUETweenStatus::Completed;
			}
		}
		else if (tween->Status == EDUETweenStatus::Completed)
		{
			ReturnTweenToPool(tween);
		}
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
