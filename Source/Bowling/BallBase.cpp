// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBase.h"

#include "Pin.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABallBase::ABallBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Physics Sphere"));
	PhysicsComponent->SetupAttachment(RootComponent);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(PhysicsComponent);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	SpringArmComp->SetupAttachment(PhysicsComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = false;
	SpringArmComp->CameraLagSpeed = 3.0f;
	SpringArmComp->SetAbsolute(false, true, false);
}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABallBase::OnPinContact(APin* pin, FHitResult hitResult)
{
	if(pin == nullptr)
	{
		return;
	}
	if(PhysicsComponent->GetPhysicsLinearVelocity().Length() < PinHitThreshold)
	{
		return;
	}
	if(pin->TouchedByBall)
	{
		return;
	}

	pin->TouchedByBall = true;

	FVector hitForce = hitResult.Normal * -1;
	
	hitForce *= PinHitForceMultiplier;

	pin->PrimitiveComponent->AddImpulseAtLocation(hitForce, hitResult.Location);

	OnSuccessfulPinHit(pin, hitResult);
}
