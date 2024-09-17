// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenBlueprintFunctionLibrary.h"

#include "FDueTweenAction.h"
#include "UObject/UnrealTypePrivate.h"

void UDueTweenBlueprintFunctionLibrary::DueMove(UObject* Target,
                                                FLatentActionInfo LatentInfo,
                                                float Duration,
                                                FVector TargetLocation,
                                                EDueEasingType DueEasingType,
                                                int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		if (const AActor* targetAsActor = Cast<AActor>(Target))
		{
			Target = targetAsActor->GetRootComponent();
		}
		// null property ref -> use location
		FProperty* propertyRef = nullptr;
		FDUETweenData tweenData;
		tweenData.Target = Target;
		tweenData.Duration = Duration;
		tweenData.EasingType = DueEasingType;
		tweenData.Steps = Steps;
		tweenData.TargetProperty = propertyRef;
		tweenData.TargetValue.SetSubtype<FVector>(TargetLocation);
		tweenData.ValueType = EDUEValueType::Vector;

		CreateAndStartLatentAction(World, LatentInfo, tweenData);
	}
}

void UDueTweenBlueprintFunctionLibrary::DueRotate(UObject* Target, FLatentActionInfo LatentInfo, float Duration,
                                                  FRotator TargetRotation,
                                                  EDueEasingType DueEasingType,
                                                  int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		if (const AActor* targetAsActor = Cast<AActor>(Target))
		{
			Target = targetAsActor->GetRootComponent();
		}
		// null property ref -> use rotation
		FProperty* propertyRef = nullptr;
		FDUETweenData tweenData;
		tweenData.Target = Target;
		tweenData.Duration = Duration;
		tweenData.EasingType = DueEasingType;
		tweenData.Steps = Steps;
		tweenData.TargetProperty = propertyRef;
		tweenData.TargetValue.SetSubtype<FRotator>(TargetRotation);
		tweenData.ValueType = EDUEValueType::Rotator;

		CreateAndStartLatentAction(World, LatentInfo, tweenData);
	}
}

void UDueTweenBlueprintFunctionLibrary::DueFloatField(UObject* Target, FLatentActionInfo LatentInfo,
                                                      FName FieldName, float Duration, float TargetValue,
                                                      EDueEasingType DueEasingType,
                                                      int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			nullptr)
		{
			FDUETweenData tweenData;
			FProperty* propertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (propertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = DueEasingType;
			tweenData.Steps = Steps;
			tweenData.TargetProperty = propertyRef;
			tweenData.TargetValue.SetSubtype<float>(TargetValue);
			tweenData.ValueType = EDUEValueType::Float;

			CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueDoubleField(UObject* Target, FLatentActionInfo LatentInfo, FName FieldName,
                                                       float Duration, double TargetValue, EDueEasingType DueEasingType,
                                                       int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			nullptr)
		{
			FDUETweenData tweenData;
			FProperty* propertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (propertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = DueEasingType;
			tweenData.Steps = Steps;
			tweenData.TargetProperty = propertyRef;
			tweenData.TargetValue.SetSubtype<double>(TargetValue);
			tweenData.ValueType = EDUEValueType::Double;

			CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueVectorField(UObject* Target, FLatentActionInfo LatentInfo, FName FieldName,
                                                       float Duration, FVector TargetValue,
                                                       EDueEasingType DueEasingType, int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			nullptr)
		{
			FDUETweenData tweenData;
			FProperty* propertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (propertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = DueEasingType;
			tweenData.Steps = Steps;
			tweenData.TargetProperty = propertyRef;
			tweenData.TargetValue.SetSubtype<FVector>(TargetValue);
			tweenData.ValueType = EDUEValueType::Vector;

			CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueRotatorField(UObject* Target, FLatentActionInfo LatentInfo, FName FieldName,
                                                        float Duration, FRotator TargetValue,
                                                        EDueEasingType DueEasingType, int32 Steps)
{
	// Prepare latent action
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
			nullptr)
		{
			FDUETweenData tweenData;
			FProperty* propertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (propertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			tweenData.Target = Target;
			tweenData.Duration = Duration;
			tweenData.EasingType = DueEasingType;
			tweenData.Steps = Steps;
			tweenData.TargetProperty = propertyRef;
			tweenData.TargetValue.SetSubtype<FRotator>(TargetValue);
			tweenData.ValueType = EDUEValueType::Vector;

			CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::CreateAndStartLatentAction(UWorld* World, FLatentActionInfo LatentInfo,
                                                                   FDUETweenData TweenData)
{
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
		nullptr)
	{
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
		                                 new FDueTweenAction(LatentInfo, TweenData));
		return;
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unable to start latent action with UUID: %d"), LatentInfo.UUID);
}
