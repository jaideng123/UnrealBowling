// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPawn.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
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
	GuideDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("GuideDecal"));
	BallPivotComp = CreateDefaultSubobject<USceneComponent>(TEXT("BallPivot"));
	BallAnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("BallAnchor"));
	BallHandComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallHand"));
	BallHandComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//Attach our components
	StaticMeshRoot->SetupAttachment(RootComponent);
	StaticMeshComp->SetupAttachment(StaticMeshRoot);

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	GuideDecalComp->SetupAttachment(RootComponent);

	BallPivotComp->SetupAttachment(RootComponent);
	BallAnchorComp->SetupAttachment(BallPivotComp);
	BallHandComp->SetupAttachment(BallAnchorComp);

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

	// TODO: Figure out why I need to hack the location to align it with the ball
	GuideDecalComp->SetRelativeLocation(
		GuideDecalComp->GetRelativeLocation() + BallSpawnOffset + GetActorRightVector() * 3);
}

// Called every frame
void ABowlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CurrentBall != nullptr)
	{
		// FVector BallPivot = GetActorLocation() + BallSpawnOffset;
		// FVector BallDown = GetActorUpVector() * -1 * ArmLength;
		if(BallGripped)
		{
			ThrowTime += DeltaTime;
			FRotator CurrentBallRotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
			BallPivotComp->SetRelativeRotation(CurrentBallRotation);
			BallPivotComp->SetRelativeLocation(BallSpawnOffset);
			// CurrentBall->SetActorLocationAndRotation(BallPivot + CurrentBallRotation.RotateVector(BallDown), CurrentBallRotation);
			DrawDebugDirectionalArrow(GetWorld(),
			                          CurrentBall->GetActorLocation(),
			                          (CurrentBall->GetActorLocation() + .1 * CurrentBall->GetActorRightVector() * CalculateBallSpin()) + CurrentBall->
			                          GetActorForwardVector() *
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
			FRotator DefaultRotation = FRotator::MakeFromEuler(FVector(0, MaxArmAngle, 0));
			// CurrentBall->SetActorLocationAndRotation(BallPivot + DefaultRotation.RotateVector(BallDown), DefaultRotation);
			BallPivotComp->SetRelativeRotation(DefaultRotation);
			BallPivotComp->SetRelativeLocation(BallSpawnOffset);
		}
	}
}

// Called to bind functionality to input
void ABowlerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABowlerPawn::MoveX(float input)
{
	if(input == 0 || BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving in direction: %f"), input);
	const FVector CurrentLocation = GetActorLocation();
	const FVector DesiredLocation = CurrentLocation + GetActorRightVector() * input;
	SetActorLocation(DesiredLocation);
}

void ABowlerPawn::MoveBallY(float input)
{
	if(!BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving Ball in direction: %f"), input);
	BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + input, MinArmAngle, MaxArmAngle);
	// If input has changed direction or is 0
	if((input >= 0) != (ThrowDistance >= 0))
	{
		ThrowDistance = 0;
		ThrowTime = 0;
		BallSpinAmount = 0;
	}
	if(BallRotationOffset != MinArmAngle && BallRotationOffset != MaxArmAngle)
	{
		ThrowDistance += input;
	}
}

void ABowlerPawn::MoveBallX(float input)
{
	if(!BallGripped)
	{
		return;
	}
	BallSpinAmount += input;
}

void ABowlerPawn::GripBall()
{
	if(CurrentBall == nullptr)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Ball gripped"));
	BallGripped = true;
	BallRotationOffset = MaxArmAngle;
}

float ABowlerPawn::CalculateBallSpin()
{
	return FMath::Clamp(BallSpinMultiplier * BallSpinAmount, -MaxBallSpin, MaxBallSpin);
}

void ABowlerPawn::ReleaseBall()
{
	if(CurrentBall == nullptr || !BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Ball released"));

	BallGripped = false;
	if(CalculateReleaseForce() == 0)
	{
		return;
	}
	GetLocalViewingPlayerController()->SetViewTargetWithBlend(CurrentBall, 0.8, VTBlend_EaseInOut, 2.0, false);
	// TODO: figure out why this is so wierd w/ SetEnablePhysics()
	CurrentBall->PhysicsComponent->SetSimulatePhysics(true);
	CurrentBall->IsActive = true;
	const auto releaseForce = CalculateReleaseForce();
	auto       forceVector = CurrentBall->GetActorForwardVector() * CalculateReleaseForce();
	forceVector.Z = FMath::Clamp(forceVector.Z, -MaxZVelocity, MaxZVelocity);
	CurrentBall->PhysicsComponent->AddImpulse(forceVector, NAME_None,
	                                          true);

	const float BallSpin = CalculateBallSpin();
	BallSpinAmount = BallSpin;
	CurrentBall->PhysicsComponent->AddAngularImpulseInDegrees(
		GetActorForwardVector() * -BallSpin, NAME_None, true);

	UE_LOG(LogTemp, Display, TEXT("Release force: %f"), releaseForce);

	if(releaseForce >= 0 && releaseForce <= 20.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("No Release Force!"));
		OnNoReleaseForce();
	}

	ThrownBall = CurrentBall;
	CurrentBall = nullptr;

	ThrowDistance = 0;
	ThrowTime = 0;
	BallSpinAmount = 0;
	BallRotationOffset = MaxArmAngle;
}

void ABowlerPawn::SpawnNewBall()
{
	if(CurrentBall != nullptr)
	{
		CurrentBall->Destroy();
	}
	CurrentBall = GetWorld()->SpawnActor<ABallBase>(BallClass, FVector::Zero(), FRotator::ZeroRotator);
	CurrentBall->PhysicsComponent->SetSimulatePhysics(false);
	CurrentBall->IsActive = false;
	const FAttachmentTransformRules ballAttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld,
	                                                    false);
	bool succeeded = CurrentBall->AttachToComponent(BallAnchorComp, ballAttachmentRules);
	if(succeeded)
	{
		UE_LOG(LogTemp, Log,TEXT("SUCESS!"));
	}
	else
	{
		UE_LOG(LogTemp, Log,TEXT("FAILURE!"));
	}
	
}

void ABowlerPawn::ResetBall()
{
	if(ThrownBall == nullptr)
	{
		return;
	}
	CurrentBall = ThrownBall;
	ThrownBall = nullptr;
	CurrentBall->PhysicsComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
	CurrentBall->PhysicsComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
	CurrentBall->PhysicsComponent->SetEnableGravity(false);
	CurrentBall->SetActorLocationAndRotation(GetActorLocation() + BallSpawnOffset, GetActorRotation());
	CurrentBall->IsActive = false;
	BallRotationOffset = MaxArmAngle;
}

float ABowlerPawn::CalculateReleaseForce() const
{
	return FMath::Clamp((ThrowDistance / (ThrowTime * ThrowForceDecay)) * BallReleaseMultiplier, -MaxBallForce,
	                    MaxBallForce);
}
