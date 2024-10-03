// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUETweenModule.h"
#include "DueTweenPool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DueTweenSubsystem.generated.h"

/**
 * World Subsystem that manages and ticks tweens
 */
UCLASS()
class DUETWEEN_API UDueTweenSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	// Creates and starts a new tween
	FActiveDueTweenHandle AddTween(FDUETweenData& TweenData);
	// Pauses a currently running tween (returns true if successful)
	bool PauseTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Resumes a paused tween (returns true if successful)
	bool ResumeTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Skips to the end of a tween (returns true if successful)
	bool FastForwardTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Skips to the end of a tween (returns true if successful)
	bool StopTween(const FActiveDueTweenHandle& TweenHandle) const;
	// Gets a pointer to a tween from its handle DO NOT SAVE THIS POINTER
	FActiveDueTween* GetTweenFromHandle(const FActiveDueTweenHandle& TweenHandle) const;
	
	// Tick
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	// NOTE: Every tickable game object needs this or it will throw an exception
	virtual TStatId GetStatId() const override { return TStatId(); }

	void TickTween(float DeltaTime, FActiveDueTween* CurrentTween);
private:
	FDueTweenPool Pool = FDueTweenPool();
	void RemoveTweenFromActiveChain(FActiveDueTweenHandle TweenHandleToRemove);
	FActiveDueTweenHandle ActiveTweenChainStart = NULL_DUETWEEN_HANDLE;
	unsigned int LastAssignedTweenId = 0;
};
