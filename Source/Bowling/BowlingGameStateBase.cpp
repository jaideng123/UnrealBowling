// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingGameStateBase.h"

#include "BowlingPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

UStateCallbacks* ABowlingGameStateBase::GetCallbacksForBowlingState(UObject* Context, TEnumAsByte<EMatchState> State)
{
	ABowlingGameStateBase* BowlingGameState = GetBowlingGameState(Context);
	if(!BowlingGameState->StateCallbacks.Contains(State))
	{
		BowlingGameState->StateCallbacks.Add(State, NewObject<UStateCallbacks>());
	}

	return BowlingGameState->StateCallbacks[State];
}

ABowlingGameStateBase* ABowlingGameStateBase::GetBowlingGameState(UObject* Context)
{
	if(Context == nullptr || Context->GetWorld() == nullptr)
	{
		UE_LOGFMT(LogTemp, Error, "Invalid context for world");
		return nullptr;
	}
	AGameStateBase* GameStateBase = Context->GetWorld()->GetGameState();
	ABowlingGameStateBase* BowlingGameState = Cast<ABowlingGameStateBase>(GameStateBase);
	if(BowlingGameState == nullptr)
	{
		UE_LOGFMT(LogTemp, Error, "Bowling Game State does not exist in this world");
		return nullptr;
	}
	return BowlingGameState;
}

int32 ABowlingGameStateBase::GetActivePlayerId(UObject* Context)
{
	ABowlingGameStateBase* BowlingGameState = GetBowlingGameState(Context);
	if(BowlingGameState != nullptr && BowlingGameState->GetActivePlayerState())
	{
		return BowlingGameState->GetActivePlayerState()->GetPlayerId();
	}
	return -1;
}

bool ABowlingGameStateBase::IsActivePlayer(UObject* Context)
{
	int32 ActivePlayerId = GetActivePlayerId(Context);
	if(ActivePlayerId == -1)
	{
		return false;
	}
	class APlayerController* FirstLocalPlayerController = GEngine->GetFirstLocalPlayerController(Context->GetWorld());
	if(FirstLocalPlayerController == nullptr || FirstLocalPlayerController->PlayerState == nullptr)
	{
		return false;
	}
	return FirstLocalPlayerController->PlayerState->GetPlayerId() == ActivePlayerId;
}

EMatchState ABowlingGameStateBase::GetMatchState() const
{
	return CurrentMatchState;
}

void ABowlingGameStateBase::OnRepMatchState(TEnumAsByte<EMatchState> PreviousState)
{
	if(PreviousState != UNSET && StateCallbacks.Contains(PreviousState))
	{
		StateCallbacks[PreviousState]->OnStateExited.Broadcast();
	}
	if(StateCallbacks.Contains(CurrentMatchState))
	{
		StateCallbacks[CurrentMatchState]->OnStateEntered.Broadcast();
	}
}

void ABowlingGameStateBase::SetMatchState(EMatchState NewMatchState)
{
	if(!HasAuthority())
	{
		UE_LOGFMT(LogTemp, Warning, "Cannot set match state without authority");
		return;
	}
	if(NewMatchState == CurrentMatchState)
	{
		UE_LOGFMT(LogTemp, Warning, "Setting match state that has already been set");
		return;
	}

	const EMatchState PreviousMatchState = CurrentMatchState;
	CurrentMatchState = NewMatchState;
	OnRepMatchState(PreviousMatchState);
}

void ABowlingGameStateBase::CyclePlayer()
{
	ActivePlayerIndex = FMath::Wrap(ActivePlayerIndex + 1, 0, PlayerArray.Num() - 1);
}

ABowlingPlayerState* ABowlingGameStateBase::GetActivePlayerState()
{
	if(ActivePlayerIndex >= PlayerArray.Num())
	{
		return nullptr;
	}
	return Cast<ABowlingPlayerState>(PlayerArray[ActivePlayerIndex]);
}

float ABowlingGameStateBase::GetFinalFrame(UWorld* WorldRef)
{
	ABowlingGameStateBase* bowlingGameMode = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(WorldRef));
	return bowlingGameMode->FinalFrame;
}

float ABowlingGameStateBase::GetNumPins(UWorld* WorldRef)
{
	ABowlingGameStateBase* bowlingGameMode = Cast<ABowlingGameStateBase>(UGameplayStatics::GetGameState(WorldRef));
	return bowlingGameMode->NumPins;
}

void ABowlingGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABowlingGameStateBase, CurrentMatchState);
	DOREPLIFETIME(ABowlingGameStateBase, ActivePlayerIndex);
}
