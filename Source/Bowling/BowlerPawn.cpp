// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlerPawn.h"

#include "BowlerPlayerController.h"
#include "DUETween.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
	MoveModeDisplayComp = CreateDefaultSubobject<USceneComponent>("MoveModeDisplay");
	RotateModeDisplayComp = CreateDefaultSubobject<USceneComponent>("RotateModeDisplay");


	//Attach our components
	StaticMeshRoot->SetupAttachment(RootComponent);
	StaticMeshComp->SetupAttachment(StaticMeshRoot);

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	GuideDecalComp->SetupAttachment(RootComponent);

	BallPivotComp->SetupAttachment(RootComponent);
	BallAnchorComp->SetupAttachment(BallPivotComp);
	BallHandComp->SetupAttachment(BallAnchorComp);

	MoveModeDisplayComp->SetupAttachment(RootComponent);
	RotateModeDisplayComp->SetupAttachment(RootComponent);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABowlerPawn::BeginPlay()
{
	Super::BeginPlay();
	if(!HasAuthority())
	{
		return;
	}
	InitialForward = GetActorForwardVector();
	InitialRight = GetActorRightVector();
	StartingOrientation = GetActorRotation();
	// Center the ball
	SetActorLocation(GetActorLocation() - BallSpawnOffset);

	// Set up for run
	SetActorLocation(GetActorLocation() - InitialForward * StartDistance);

	StartingPosition = GetActorLocation();

	SpringArmComp->TargetOffset = BallSpawnOffset;

	GuideDecalComp->SetRelativeLocation(
		GuideDecalComp->GetRelativeLocation() + BallSpawnOffset + InitialRight * 3);

	MoveTweenHandle = FActiveDUETweenHandle::NULL_HANDLE();
	UpdateMovementModeDisplay();
}

void ABowlerPawn::HideUI()
{
	AController* localPlayerController = GetController();
	if(ABowlerPlayerController* bowlingPlayerController = Cast<ABowlerPlayerController>(localPlayerController))
	{
		bowlingPlayerController->HideUI();
	}
}

void ABowlerPawn::ShowUI()
{
	AController* localPlayerController = GetController();
	if(ABowlerPlayerController* bowlingPlayerController = Cast<ABowlerPlayerController>(localPlayerController))
	{
		bowlingPlayerController->ShowUI();
	}
}

void ABowlerPawn::ShowPreBowlUI()
{
	AController* localPlayerController = GetController();
	if(ABowlerPlayerController* bowlingPlayerController = Cast<ABowlerPlayerController>(localPlayerController))
	{
		bowlingPlayerController->ShowPreBowlUI();
	}
}

void ABowlerPawn::ShowControlUI()
{
	AController* localPlayerController = GetController();
	if(ABowlerPlayerController* bowlingPlayerController = Cast<ABowlerPlayerController>(localPlayerController))
	{
		bowlingPlayerController->ShowControlUI();
	}
}

void ABowlerPawn::ShowEndUI()
{
	AController* localPlayerController = GetController();
	if(ABowlerPlayerController* bowlingPlayerController = Cast<ABowlerPlayerController>(localPlayerController))
	{
		bowlingPlayerController->ShowEndUI();
	}
}

void ABowlerPawn::PossiblyStartRunUpTween()
{
	if(MoveTweenHandle == nullptr)
	{
		const FVector TargetLocation = BallGripStartPosition.GetValue() + GetActorForwardVector() *
			StartDistance;
		const FVector StartPosition = BallGripStartPosition.GetValue();
		FTweenUpdateCallback TargetCallback = [TargetLocation,StartPosition,Curve = RunUpCurve.GetRichCurveConst(), this
			](
			const FValueContainer& UpdatedValue)
		{
			const FVector CurrentLocation = FMath::Lerp(StartPosition, TargetLocation,
			                                            Curve->Eval(UpdatedValue.GetSubtype<float>()));
			this->SetActorLocation(CurrentLocation);
		};
		MoveTweenHandle = DUETween::StartDUETween<float>(this, TargetCallback, 0.0, 1.0, RunUpTimeMS,
		                                                 EDueEasingType::Linear);
	}
}

void ABowlerPawn::CancelRunUpTween()
{
	if(MoveTweenHandle != nullptr)
	{
		MoveTweenHandle.StopTween();
		MoveTweenHandle = FActiveDUETweenHandle::NULL_HANDLE();
	}
}

// Called every frame
void ABowlerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!HasAuthority())
	{
		return;
	}

	if(CurrentBall != nullptr)
	{
		if(BallGripped)
		{
			check(BallGripStartPosition.IsSet());
			PossiblyStartRunUpTween();

			ThrowTime += DeltaTime;
			GrippedTime += DeltaTime;
			FRotator CurrentPivotRotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
			BallPivotComp->SetRelativeRotation(CurrentPivotRotation);
			BallPivotComp->SetRelativeLocation(BallSpawnOffset);
			FVector newLocation = BallHandComp->GetRelativeLocation();
			newLocation.Y = -1 * (CalculateBallSpin() / MaxBallSpin) * MaxHandShift;
			BallHandComp->SetRelativeLocation(newLocation);

			// DrawDebugDirectionalArrow(GetWorld(),
			//                           CurrentBall->GetActorLocation(),
			//                           (CurrentBall->GetActorLocation() + .1 * CurrentBall->GetActorRightVector() * CalculateBallSpin()) + CurrentBall->
			//                           GetActorForwardVector() *
			//                           (10 + 100 * (CalculateReleaseForce() / MaxBallForce)),
			//                           100,
			//                           FMath::Lerp<FLinearColor>(FLinearColor::Green, FLinearColor::Red,
			//                                                     FMath::Abs(CalculateReleaseForce()) / MaxBallForce)
			//                           .QuantizeFloor(),
			//                           false,
			//                           -1,
			//                           1,
			//                           5);
		}
		else
		{
			// Reset to resting position
			// TODO: convert to tunable var
			BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + (DeltaTime * 100.0f), MinArmAngle,
			                                         MaxArmAngle);
			FRotator CurrentBallRotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
			BallPivotComp->SetRelativeRotation(CurrentBallRotation);
			BallPivotComp->SetRelativeLocation(BallSpawnOffset);

			if(BallGripStartPosition.IsSet())
			{
				CancelRunUpTween();
				SetActorLocation(BallGripStartPosition.GetValue());
				BallGripStartPosition.Reset();
				OnMoveReset();
			}
		}
	}
	else
	{
		// Follow Through
		// TODO: convert to tunable var
		BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + (DeltaTime * 250.0f), MinArmAngle, MaxArmAngle);
		FRotator CurrentBallRotation = FRotator::MakeFromEuler(FVector(0, BallRotationOffset, 0));
		BallPivotComp->SetRelativeRotation(CurrentBallRotation);
		BallPivotComp->SetRelativeLocation(BallSpawnOffset);
	}
}

// Called to bind functionality to input
void ABowlerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABowlerPawn::MoveX_Implementation(float input)
{
	if(!HasAuthority())
	{
		return;
	}
	OnMove(input);
	if(input == 0 || BallGripped)
	{
		return;
	}
	if(CurrentMovementMode == MOVE)
	{
		UE_LOG(LogTemp, Display, TEXT("Moving in direction: %f"), input);
		const FVector CurrentLocation = GetActorLocation();
		const FVector Offset = InitialRight * input * StrafingSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
		const FVector DesiredLocation = CurrentLocation + Offset;
		SetActorLocation(DesiredLocation);
	}
	else if(CurrentMovementMode == ROTATE)
	{
		UE_LOG(LogTemp, Display, TEXT("Rotating in direction: %f"), input);
		FRotator CurrentRotation = GetActorRotation();
		float Offset = input * RotationSpeedDegrees * UGameplayStatics::GetWorldDeltaSeconds(this);
		CurrentRotation.Yaw = FMath::Clamp(CurrentRotation.Yaw + Offset, -MaxRotation, MaxRotation);
		SetActorRotation(CurrentRotation);
	}
	else
	{
		unimplemented();
	}
}

void ABowlerPawn::MoveBallY_Implementation(float input)
{
	if(!HasAuthority())
	{
		return;
	}
	if(!BallGripped || FMath::Abs(input) < .001)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Moving Ball in direction: %f"), input);
	BallRotationOffset = FMath::Clamp<float>(BallRotationOffset + input, MinArmAngle, MaxArmAngle);
	// If input has changed direction 
	if(FMath::Sign(input) != FMath::Sign(ThrowDistance))
	{
		UE_LOG(LogTemp, Display, TEXT("Resetting Ball Force"));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Resetting Ball Force")));
		ThrowDistance = 0;
		ThrowTime = 0;
	}
	ThrowDistance += input;
	if(input < 0.0f)
	{
		ThrowWindupDistance += input;
		if(!ThrowWindupThresholdPassed && FMath::Abs(ThrowWindupDistance) > WindupDistanceThreshold)
		{
			OnWindupThresholdPassed();
			ThrowWindupThresholdPassed = true;
		}
	}
}

void ABowlerPawn::MoveBallX_Implementation(float input)
{
	if(!HasAuthority())
	{
		return;
	}
	if(!BallGripped)
	{
		return;
	}
	BallSpinAmount += input;
}

void ABowlerPawn::GripBall_Implementation()
{
	if(!HasAuthority())
	{
		return;
	}
	if(CurrentBall == nullptr || BowlingLocked)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Ball gripped"));
	BallGripped = true;
	BallGripStartPosition = GetActorLocation();
	GuideDecalComp->SetVisibility(false);
	HideMovementModeDisplay();
	HideUI();
	BallRotationOffset = MaxArmAngle;
	OnGrip();
}

float ABowlerPawn::CalculateBallSpin()
{
	return FMath::Clamp(BallSpinMultiplier * BallSpinAmount, -MaxBallSpin, MaxBallSpin);
}

void ABowlerPawn::ResetBallGripState()
{
	ThrowDistance = 0;
	ThrowWindupDistance = 0;
	ThrowWindupThresholdPassed = false;
	ThrowTime = 0;
	BallSpinAmount = 0;
	GrippedTime = 0;
}

void ABowlerPawn::ReleaseBall_Implementation()
{
	if(!HasAuthority())
	{
		return;
	}

	if(CurrentBall == nullptr || !BallGripped)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Ball released"));
	BallGripped = false;
	if(FMath::Abs(CalculateReleaseForce()) < ReleaseForceThreshold || !ThrowWindupThresholdPassed)
	{
		GuideDecalComp->SetVisibility(true);
		UpdateMovementModeDisplay();
		ShowUI();
		ResetBallGripState();
		OnNoReleaseForce();
		return;
	}
	BallGripStartPosition.Reset();
	CancelRunUpTween();
	OnRelease();
	// TODO: Fix This
	// GetLocalViewingPlayerController()->SetViewTargetWithBlend(CurrentBall, 0.8, VTBlend_EaseInOut, 2.0, false);

	CurrentBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentBall->PhysicsComponent->SetSimulatePhysics(true);
	CurrentBall->PhysicsComponent->SetPhysicsLinearVelocity(FVector::Zero());
	CurrentBall->IsActive = true;
	float releaseForce = CalculateReleaseForce();
	if(releaseForce >= 0 && releaseForce < MinBallForce)
	{
		releaseForce = MinBallForce;
	}
	else if(releaseForce < 0 && releaseForce > -MinBallForce)
	{
		releaseForce = -MinBallForce;
	}
	auto forceVector = CurrentBall->GetActorForwardVector() * releaseForce;
	if(releaseForce > 0)
	{
		forceVector.Z = FMath::Clamp(forceVector.Z, -MaxZVelocity, MaxZVelocity);
	}
	else
	{
		forceVector.Z = FMath::Clamp(forceVector.Z, -MaxZVelocity, MaxZVelocity * 3);
		forceVector.X *= 0.5f;
	}
	CurrentBall->PhysicsComponent->SetPhysicsLinearVelocity(forceVector);

	const float BallSpin = CalculateBallSpin();
	BallSpinAmount = BallSpin;
	CurrentBall->PhysicsComponent->SetPhysicsAngularVelocityInDegrees(
		GetActorForwardVector() * -BallSpin);

	UE_LOG(LogTemp, Display, TEXT("Release force: %f / %f"), releaseForce, MaxBallForce);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
	//                                  FString::Printf(TEXT("Release force: %f / %f"), releaseForce, MaxBallForce));

	// Ensure we are releasing with at least some force
	check(FMath::Abs(releaseForce) > 0);

	ThrownBall = CurrentBall;
	CurrentBall = nullptr;

	ResetBallGripState();
}

void ABowlerPawn::UpdateMovementModeDisplay() const
{
	if(!HasAuthority())
	{
		return;
	}
	if(CurrentMovementMode == EBowlerMovementMode::MOVE)
	{
		RotateModeDisplayComp->SetVisibility(false, true);
		MoveModeDisplayComp->SetVisibility(true, true);
	}
	else
	{
		RotateModeDisplayComp->SetVisibility(true, true);
		MoveModeDisplayComp->SetVisibility(false, true);
	}
}

void ABowlerPawn::HideMovementModeDisplay() const
{
	if(!HasAuthority())
	{
		return;
	}
	RotateModeDisplayComp->SetVisibility(false, true);
	MoveModeDisplayComp->SetVisibility(false, true);
}

void ABowlerPawn::ToggleMovementMode_Implementation()
{
	if(!HasAuthority())
	{
		return;
	}
	if(CurrentMovementMode == EBowlerMovementMode::MOVE)
	{
		CurrentMovementMode = EBowlerMovementMode::ROTATE;
	}
	else
	{
		CurrentMovementMode = EBowlerMovementMode::MOVE;
	}
	UpdateMovementModeDisplay();
}

void ABowlerPawn::AttachBallToHand()
{
	const FAttachmentTransformRules ballAttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,
	                                                    EAttachmentRule::KeepWorld,
	                                                    false);
	bool IsAttached = CurrentBall->AttachToComponent(BallAnchorComp, ballAttachmentRules);
	check(IsAttached);
}

void ABowlerPawn::SpawnNewBall()
{
	if(!HasAuthority())
	{
		return;
	}
	if(CurrentBall != nullptr)
	{
		CurrentBall->Destroy();
	}
	CurrentBall = GetWorld()->SpawnActor<ABallBase>(BallClass, FVector::Zero(), FRotator::ZeroRotator);
	CurrentBall->PhysicsComponent->SetSimulatePhysics(false);
	CurrentBall->IsActive = false;
	AttachBallToHand();
}

void ABowlerPawn::ResetBall()
{
	if(!HasAuthority())
	{
		return;
	}
	if(ThrownBall == nullptr)
	{
		return;
	}
	CurrentBall = ThrownBall;
	ThrownBall = nullptr;
	CurrentBall->PhysicsComponent->SetAllPhysicsLinearVelocity(FVector::Zero());
	CurrentBall->PhysicsComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
	CurrentBall->PhysicsComponent->SetSimulatePhysics(false);
	CurrentBall->SetActorLocationAndRotation(FVector::Zero(), FRotator::ZeroRotator, false, nullptr,
	                                         ETeleportType::ResetPhysics);
	CurrentBall->IsActive = false;
	GuideDecalComp->SetVisibility(true);
	UpdateMovementModeDisplay();
	AttachBallToHand();
	// BallRotationOffset = MaxArmAngle;
	SetActorLocation(StartingPosition);
	SetActorRotation(StartingOrientation);
}

void ABowlerPawn::OnMove_Implementation(float moveDist)
{
}

float ABowlerPawn::CalculateReleaseForce() const
{
	return FMath::Clamp((ThrowDistance / (ThrowTime * ThrowForceDecay)) * BallReleaseMultiplier, -MaxBallForce,
	                    MaxBallForce);
}
