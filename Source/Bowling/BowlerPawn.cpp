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

// Called when the game starts or when spawned
void ABowlerPawn::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComp->TargetOffset = BallSpawnOffset;

	CurrentBall = GetWorld()->SpawnActor<
		ABallBase>(BallClass, GetActorLocation() + BallSpawnOffset, GetActorRotation());
	CurrentBall->PhysicsComponent->SetEnableGravity(false);
}

// Called every frame
void ABowlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentBall != nullptr)
	{
		FVector BallPivot = GetActorLocation() + BallSpawnOffset;
		FVector BallDown = GetActorUpVector() * -1 * 50;
		if (BallGripped)
		{
			FRotator Rotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
			FVector LastBallPosition = CurrentBall->GetActorLocation();
			// CurrentBall->PhysicsComponent->MoveComponentImpl(
			// 	(BallPivot + Rotation.RotateVector(BallDown)) - CurrentBall->GetActorLocation(), Rotation.Quaternion(),
			// 	false);
			CurrentBall->SetActorLocationAndRotation(BallPivot + Rotation.RotateVector(BallDown), Rotation);
			// Need to figure out a better way to accumulate this
			// BallVelocity = FVector::Distance(LastBallPosition, CurrentBall->GetActorLocation()) / DeltaTime;
		}
		else
		{
			BallVelocity = 1;
			CurrentBall->SetActorLocation(BallPivot);
		}
		DrawDebugDirectionalArrow(GetWorld(), CurrentBall->GetActorLocation(),
		                          CurrentBall->GetActorLocation() + CurrentBall->GetActorForwardVector() * 100, 100,
		                          FColor::Green,
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
	if (value == 0 || BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving in direction: %f"), value);
	FVector Location = GetActorLocation();
	Location += GetActorRightVector() * value;
	SetActorLocation(Location);
}

void ABowlerPawn::MoveBallY(float value)
{
	if (value == 0 || !BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving Ball in direction: %f"), value);
	BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + value, -110, 110);
}

void ABowlerPawn::GripBall()
{
	UE_LOG(LogTemp, Display, TEXT("Ball gripped"));
	BallGripped = true;
}

void ABowlerPawn::ReleaseBall()
{
	if (CurrentBall == nullptr || !BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Ball released"));
	BallGripped = false;
	CurrentBall->PhysicsComponent->SetEnableGravity(true);
	CurrentBall->PhysicsComponent->AddImpulse(CurrentBall->GetActorForwardVector() * 1000, NAME_None,true);
	CurrentBall = nullptr;
}
