// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinSpawnPoint.h"
#include "GameFramework/Actor.h"
#include "PinSetter.generated.h"

class APin;

UCLASS()
class BOWLING_API APinSetter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APinSetter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<APinSpawnPoint*> PinSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APin> PinType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<APin*> SpawnedPins;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<APin*> RaisedPins;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SpawnPins();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, CallInEditor)
	void SweepPins();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ResetPins();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void RaiseStandingPins();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void LowerStandingPins();

	UFUNCTION(BlueprintCallable)
	void ReportPins();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
