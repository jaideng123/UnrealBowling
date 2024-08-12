#include "DUETween.h"

#define LOCTEXT_NAMESPACE "FDUETweenModule"

void FDUETweenModule::StartupModule()
{
	UE_LOG(LogTemp, Display, TEXT("Loaded DUETween"));

	TickDelegate = FTickerDelegate::CreateRaw(this, &FDUETweenModule::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void FDUETweenModule::ShutdownModule()
{
	UE_LOG(LogTemp, Display, TEXT("UnLoaded DUETween"));

	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

bool FDUETweenModule::Tick(float deltaTime)
{
	// Only Run Tweans During Play
	if (GEngine->GetCurrentPlayWorld())
	{
		return true;
	}

	UE_LOG(LogTemp, Display, TEXT("Ticking DUETween"));

	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDUETweenModule, DUETween)
