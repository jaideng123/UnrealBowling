// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinSpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Pin.generated.h"

UCLASS()
class BOWLING_API APin : public AActor {
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleInstanceOnly)
	UPrimitiveComponent* PrimitiveComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	APinSpawnPoint* OriginalSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ShowCenterOfMass;
	
	// Sets default values for this actor's properties
	APin();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void RaisePin(double X);

	UFUNCTION(BlueprintNativeEvent)
	void LowerPin();
	UFUNCTION(BlueprintCallable)
	void ResetToSpawn();

	UFUNCTION(BlueprintCallable)
	bool IsToppled() const;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool TouchedByBall;
};
