#include "DUETween.h"

#include "DUEEasingFunctionLibrary.h"

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

FActiveDueTween* FDUETweenModule::AddTween(const FDUETweenData& TweenData)
{
	for (auto& tween : ActiveTweens)
	{
		if (!tween.IsActive)
		{
			tween.TweenData = TweenData;
			tween.IsActive = true;
			tween.TimeElapsed = 0;
			// TODO Break Up By Type
			auto FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty);
			if(FloatProperty && TweenData.Target.IsValid())
			{
				float StartingValue;
				FloatProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);
				
				tween.StartingValue = FValueContainer(StartingValue);
			}
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
														  tween.TweenData.TargetValue.GetSubtype<float>(), progress,
														  tween.TweenData.EasingType, tween.TweenData.BlendExp,
														  tween.TweenData.Steps);
					UE_LOG(LogTemp, Display, TEXT("Actual New Value: %f"), newValue);

					FFloatProperty* FloatProperty = CastField<FFloatProperty>(tween.TweenData.TargetProperty);
					if (FloatProperty && tween.TweenData.Target.IsValid())
					{
						FloatProperty->SetValue_InContainer(tween.TweenData.Target.Get(), newValue);
					}
					break;
				}
			// case EValueType::Double:
			// 	break;
			// case EValueType::Vector:
			// 	break;
			// case EValueType::Rotator:
			// 	break;
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
