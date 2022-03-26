// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPawn.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABowlerPawn::ABowlerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("StaticMeshRoot"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//Attach our components
	StaticMeshRoot->SetupAttachment(RootComponent);
	StaticMeshComp->SetupAttachment(StaticMeshRoot);

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
}

// Called when the game starts or when spawned2
void ABowlerPawn::BeginPlay()
{
	Super::BeginPlay();
	CurrentBall = GetWorld()->SpawnActor<
		ABallBase>(BallClass, GetActorLocation() + BallSpawnOffset, GetActorRotation());
	CurrentBall->PhysicsComponent->SetSimulatePhysics(false);
}

// Called every frame
void ABowlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentBall != nullptr)
	{
		CurrentBall->SetActorLocationAndRotation(GetActorLocation() + BallSpawnOffset, GetActorRotation());
		DrawDebugDirectionalArrow(GetWorld(), CurrentBall->GetActorLocation(),
		                          CurrentBall->GetActorLocation() + GetActorForwardVector() * 100, 100, FColor::Green,
		                          false, -1, 1, 5);
	}
}

// Called to bind functionality to input
void ABowlerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABowlerPawn::MoveX(float value)
{
	if (value != 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Moving in direction: %f"), value);
	}
	FVector Location = GetActorLocation();
	Location += GetActorRightVector() * value;
	SetActorLocation(Location);
}

void ABowlerPawn::GripBall()
{
	UE_LOG(LogTemp, Display, TEXT("Ball gripped"));
}

void ABowlerPawn::ReleaseBall()
{
	if (CurrentBall == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Ball released"));
	CurrentBall->PhysicsComponent->SetSimulatePhysics(true);
	CurrentBall->PhysicsComponent->AddImpulse(GetActorForwardVector() * 1000, NAME_None, true);
	CurrentBall = nullptr;
}
