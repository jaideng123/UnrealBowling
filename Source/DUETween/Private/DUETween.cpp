#include "DUETween.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
	ActiveTweens.SetNum(100);
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
		if(!tween.IsActive)
		{
			tween.TweenData = TweenData;
			tween.IsActive = true;
			tween.TimeElapsed = 0;
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

	for (auto tween : ActiveTweens)
	{
		if(tween.IsActive)
		{
			
			switch (tween.TweenData.ValueType) {
			case EValueType::Float:
				break;
			case EValueType::Double:
				break;
			case EValueType::Vector:
				break;
			case EValueType::Rotator:
				break;
			}
		}
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
