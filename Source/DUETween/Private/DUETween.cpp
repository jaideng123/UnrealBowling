#include "DUETween.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

DEFINE_LOG_CATEGORY(LogDUETween);

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogDUETween, Display, TEXT("Loaded DUETween"));

	TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void FDUETweenModule::ShutdownModule()
{
	UE_LOG(LogDUETween, Display, TEXT("UnLoaded DUETween"));

	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

bool FDUETweenModule::Tick(float deltaTime)
{
	// Only Run Tweans During Play
	if (GEngine->GetCurrentPlayWorld())
	{
		return true;
	}

	// UE_LOG(LogDUETween, Display, TEXT("Ticking DUETween"));

	for (auto Tween : ActiveTweens)
	{
		// TODO: Need to figure out updating this
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
