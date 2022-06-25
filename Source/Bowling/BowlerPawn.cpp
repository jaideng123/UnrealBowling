// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPawn.h"

#include "Kismet/KismetSystemLibrary.h"
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

	// Center the ball
	SetActorLocation(GetActorLocation() - BallSpawnOffset);

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
		FVector BallDown = GetActorUpVector() * -1 * 60;
		if (BallGripped)
		{
			ThrowTime += DeltaTime;
			FRotator Rotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
			CurrentBall->SetActorLocationAndRotation(BallPivot + Rotation.RotateVector(BallDown), Rotation);
			DrawDebugDirectionalArrow(GetWorld(),
			                          CurrentBall->GetActorLocation(),
			                          CurrentBall->GetActorLocation() + CurrentBall->GetActorForwardVector() *
			                          (10 + 100 * (CalculateReleaseForce() / MaxBallForce)),
			                          100,
			                          FMath::Lerp<FLinearColor>(FLinearColor::Green, FLinearColor::Red,
			                                                    FMath::Abs(CalculateReleaseForce()) / MaxBallForce)
			                          .QuantizeFloor(),
			                          false,
			                          -1,
			                          1,
			                          5);
		}
		else
		{
			CurrentBall->SetActorLocation(BallPivot + BallDown);
		}
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
	if (!BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving Ball in direction: %f"), value);
	// TODO: add decay factor
	BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + value, -110, 110);
	if ((value >= 0) != (ThrowDistance >= 0) || value == 0)
	{
		ThrowDistance = 0;
		ThrowTime = 0;
		BallSpinAmount = 0;
	}
	ThrowDistance += value;
}

void ABowlerPawn::MoveBallX(float value)
{
	if (!BallGripped)
	{
		return;
	}
	BallSpinAmount += value;
}

void ABowlerPawn::GripBall()
{
	if (CurrentBall == nullptr)
	{
		return;
	}
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
	GetLocalViewingPlayerController()->SetViewTargetWithBlend(CurrentBall, 1.0, VTBlend_EaseInOut, 2.0);
	CurrentBall->PhysicsComponent->SetEnableGravity(true);
	CurrentBall->PhysicsComponent->AddImpulse(
		CurrentBall->GetActorForwardVector() * CalculateReleaseForce(), NAME_None,
		true);
	CurrentBall->PhysicsComponent->AddAngularImpulseInDegrees(GetActorForwardVector()*-30*BallSpinAmount, NAME_None, true);
	CurrentBall = nullptr;
}

float ABowlerPawn::CalculateReleaseForce() const
{
	return FMath::Clamp((ThrowDistance / ThrowTime) * BallReleaseMultiplier, -MaxBallForce,
	                    MaxBallForce);
}
