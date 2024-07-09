// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBase.generated.h"

UCLASS()
class BOWLING_API ABallBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PinHitThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PinHitForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Effects")
	FVector2D SpeedFovRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Effects")
	UCurveFloat* SpeedFovCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Speed Effects")
	float SpeedFovMaxSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	bool IsActive;

	// Sets default values for this actor's properties
	ABallBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnPinContact(APin* pin, FHitResult hitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSuccessfulPinHit(APin* pin, FHitResult hitResult);
};
