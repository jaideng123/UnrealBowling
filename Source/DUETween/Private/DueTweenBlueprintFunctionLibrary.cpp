// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenBlueprintFunctionLibrary.h"

#include "FDueTweenAction.h"
#include "UObject/UnrealTypePrivate.h"

void UDueTweenBlueprintFunctionLibrary::DueMove(UObject* Target,
                                                const FLatentActionInfo LatentInfo,
                                                const float Duration,
                                                const FVector TargetLocation,
                                                const EDueEasingType DueEasingType,
                                                int& OutHandle,
                                                const int32 Steps)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
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

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
	}
}

void UDueTweenBlueprintFunctionLibrary::DueRotate(UObject* Target,
                                                  const FLatentActionInfo LatentInfo,
                                                  const float Duration,
                                                  const FRotator TargetRotation,
                                                  const EDueEasingType DueEasingType,
                                                  int& OutHandle,
                                                  const int32 Steps)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
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

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
	}
}

void UDueTweenBlueprintFunctionLibrary::DueFloatField(UObject* Target,
                                                      const FLatentActionInfo LatentInfo,
                                                      const FName FieldName,
                                                      const float Duration,
                                                      const float TargetValue,
                                                      const EDueEasingType DueEasingType,
                                                      int& OutHandle,
                                                      const int32 Steps)
{
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

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueDoubleField(UObject* Target,
                                                       const FLatentActionInfo LatentInfo,
                                                       const FName FieldName,
                                                       const float Duration,
                                                       const double TargetValue,
                                                       const EDueEasingType DueEasingType,
                                                       int& OutHandle,
                                                       const int32 Steps)
{
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

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueVectorField(UObject* Target,
                                                       const FLatentActionInfo LatentInfo,
                                                       const FName FieldName,
                                                       const float Duration,
                                                       const FVector TargetValue,
                                                       const EDueEasingType DueEasingType,
                                                       int& OutHandle,
                                                       const int32 Steps)
{
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

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueRotatorField(UObject* Target,
                                                        const FLatentActionInfo LatentInfo,
                                                        const FName FieldName,
                                                        const float Duration,
                                                        const FRotator TargetValue,
                                                        const EDueEasingType DueEasingType,
                                                        int& OutHandle,
                                                        const int32 Steps)
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

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

bool UDueTweenBlueprintFunctionLibrary::PauseDueTween(const int& DueTweenHandle)
{
	return FDUETweenModule::Get().PauseTween(DueTweenHandle);
}

bool UDueTweenBlueprintFunctionLibrary::ResumeDueTween(const int& DueTweenHandle)
{
	return FDUETweenModule::Get().ResumeTween(DueTweenHandle);
}

bool UDueTweenBlueprintFunctionLibrary::FastForwardDueTween(const int& DueTweenHandle)
{
	return FDUETweenModule::Get().FastForwardTween(DueTweenHandle);
}

bool UDueTweenBlueprintFunctionLibrary::StopDueTween(const int& DueTweenHandle)
{
	return FDUETweenModule::Get().StopTween(DueTweenHandle);
}

FActiveDueTweenHandle UDueTweenBlueprintFunctionLibrary::CreateAndStartLatentAction(
	UWorld* World, const FLatentActionInfo& LatentInfo,
	const FDUETweenData& TweenData)
{
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
		nullptr)
	{
		const FActiveDueTweenHandle NewHandle = FDUETweenModule::Get().AddTween(TweenData);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
		                                 new FDueTweenAction(LatentInfo, NewHandle));
		UE_LOG(LogDUETween, Verbose, TEXT("Starting latent due tween action with UUID: %d"), LatentInfo.UUID);
		return NewHandle;
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unable to start due tween latent action with UUID: %d"), LatentInfo.UUID);
	return NULL_DUETWEEN_HANDLE;
}
