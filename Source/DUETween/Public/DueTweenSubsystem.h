// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DUETween.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DueTweenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DUETWEEN_API UDueTweenSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	
	// Tick
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	// NOTE: Every tickable game object needs this or it will throw an exception
	virtual TStatId GetStatId() const override { return TStatId(); }
};
