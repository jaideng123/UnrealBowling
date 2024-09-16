#include "DUETween.h"

#include "DUEEasingFunctionLibrary.h"
#include "UObject/UnrealTypePrivate.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 0.0f);
	ActiveTweens.SetNum(100);
	for (auto& Element : ActiveTweens)
	{
		Element.IsActive = false;
	}
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
			if(TweenData.TargetProperty == nullptr)
			{
				const USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
				return FValueContainer(targetAsSceneComponent->GetRelativeLocation());
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
			if(TweenData.TargetProperty == nullptr)
			{
				USceneComponent* targetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
				targetAsSceneComponent->SetRelativeLocation(newValue.GetSubtype<FVector>());
				return;
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
			auto RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			if (RotatorProperty && RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(TweenData.Target.Get());
				*StructAddress = newValue.GetSubtype<FRotator>();
			}
			break;
		}
	}
}

FActiveDueTween* FDUETweenModule::AddTween(const FDUETweenData& TweenData)
{
	for (auto& tween : ActiveTweens)
	{
		if (!tween.IsActive)
		{
			tween.TweenData = TweenData;
			tween.IsActive = true;
			tween.TimeElapsed = 0;
			tween.StartingValue = GetCurrentValueFromProperty(TweenData);
			// TODO: check if type is wrong
			return &tween;
		}
	}
	return nullptr;
}

bool FDUETweenModule::Tick(float deltaTime)
{
	// Only Run Tweans During Play
	if (GEngine->GetCurrentPlayWorld())
	{
		return true;
	}

	// UE_LOG(LogDUETween, Display, TEXT("Ticking DUETween"));

	for (auto& tween : ActiveTweens)
	{
		if (tween.IsActive)
		{
			tween.TimeElapsed += deltaTime;

			float progress = tween.TimeElapsed / tween.TweenData.Duration;
			// UE_LOG(LogTemp, Display, TEXT("Actual Percentage Complete: %f"), progress);


			switch (tween.TweenData.ValueType)
			{
			case EDUEValueType::Float:
				{
					float newValue = DUEEasingFunctionLibrary::Ease(tween.StartingValue.GetSubtype<float>(),
					                                                tween.TweenData.TargetValue.GetSubtype<float>(),
					                                                progress,
					                                                tween.TweenData.EasingType, tween.TweenData.Steps);
					UE_LOG(LogTemp, Display, TEXT("Actual New Value: %f"), newValue);
					SetCurrentValueToProperty(tween.TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Double:
				{
					double newValue = DUEEasingFunctionLibrary::Ease(tween.StartingValue.GetSubtype<double>(),
					                                                 tween.TweenData.TargetValue.GetSubtype<double>(),
					                                                 progress,
					                                                 tween.TweenData.EasingType, tween.TweenData.Steps);
					UE_LOG(LogTemp, Display, TEXT("Actual New Value: %f"), newValue);
					SetCurrentValueToProperty(tween.TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Vector:
				{
					double ease = DUEEasingFunctionLibrary::Ease(0,
					                                             1.0,
					                                             progress,
					                                             tween.TweenData.EasingType, tween.TweenData.Steps);
					FVector newValue = FMath::Lerp(tween.StartingValue.GetSubtype<FVector>(),
					                               tween.TweenData.TargetValue.GetSubtype<FVector>(), ease);
					UE_LOG(LogTemp, Display, TEXT("Actual New Value: %s"), *newValue.ToString());
					SetCurrentValueToProperty(tween.TweenData, FValueContainer(newValue));
					break;
				}
			case EDUEValueType::Rotator:
				{
					double ease = DUEEasingFunctionLibrary::Ease(0,
					                                             1.0,
					                                             progress,
					                                             tween.TweenData.EasingType, tween.TweenData.Steps);
					FRotator newValue = FRotator();
					FQuat::Slerp(tween.StartingValue.GetSubtype<FRotator>().Quaternion(),
					             tween.TweenData.TargetValue.GetSubtype<FRotator>().Quaternion(), ease);

					UE_LOG(LogTemp, Display, TEXT("Actual New Value: %s"), *newValue.ToString());
					SetCurrentValueToProperty(tween.TweenData, FValueContainer(newValue));
				}
				break;
			default:
				UE_LOG(LogDUETween, Error, TEXT("Unhandled Data type"));
				break;
			}
			if (progress >= 1.0)
			{
				tween.IsActive = false;
			}
		}
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
