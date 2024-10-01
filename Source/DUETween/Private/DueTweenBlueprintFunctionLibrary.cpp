// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenBlueprintFunctionLibrary.h"

#include "DueTweenSubsystem.h"
#include "DueTweenAction.h"

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
		tweenData.ValueType = EDueValueType::Vector;
		TWeakObjectPtr<UObject> TargetWeakPtr = tweenData.Target;
		if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
			TargetAsSceneComponent)
		{
			tweenData.StartingValue.SetSubtype<FVector>(TargetAsSceneComponent->GetRelativeLocation());
			tweenData.TargetCallback = [](const FValueContainer& UpdatedValue, const FDUETweenData& NewTweenData)
			{
				USceneComponent* SceneComp = Cast<USceneComponent>(NewTweenData.Target.Get());
				SceneComp->SetRelativeLocation(UpdatedValue.GetSubtype<FVector>());
			};
		}
		if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
		{
			tweenData.StartingValue.SetSubtype<FVector>(TargetAsActor->GetActorLocation());
			tweenData.TargetCallback = [](const FValueContainer& UpdatedValue, const FDUETweenData& NewTweenData)
			{
				AActor* TargetActor = Cast<AActor>(NewTweenData.Target.Get());
				TargetActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
			};
		}

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
	}
}

void UDueTweenBlueprintFunctionLibrary::DueMove2D(UObject* Target,
                                                  FLatentActionInfo LatentInfo,
                                                  float Duration,
                                                  FVector2D TargetValue,
                                                  EDueEasingType DueEasingType,
                                                  int& OutHandle,
                                                  int32 Steps)
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
		tweenData.TargetValue.SetSubtype<FVector2D>(TargetValue);
		tweenData.ValueType = EDueValueType::Vector2D;

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
		tweenData.ValueType = EDueValueType::Rotator;

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
			tweenData.ValueType = EDueValueType::Float;

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
			tweenData.ValueType = EDueValueType::Double;

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
			tweenData.ValueType = EDueValueType::Vector;

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
			tweenData.ValueType = EDueValueType::Vector;

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::DueVector2DField(UObject* Target,
                                                         FLatentActionInfo LatentInfo, FName FieldName,
                                                         float Duration,
                                                         FVector2D TargetValue,
                                                         EDueEasingType DueEasingType,
                                                         int& OutHandle,
                                                         int32 Steps)
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
			tweenData.TargetValue.SetSubtype<FVector2D>(TargetValue);
			tweenData.ValueType = EDueValueType::Vector2D;

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, tweenData);
		}
	}
}

void UDueTweenBlueprintFunctionLibrary::PauseDueTween(UObject* Target, const int& DueTweenHandle, bool& Success)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		Success = World->GetSubsystem<UDueTweenSubsystem>()->PauseTween(DueTweenHandle);
	}
	Success = false;
}

void UDueTweenBlueprintFunctionLibrary::ResumeDueTween(UObject* Target, const int& DueTweenHandle, bool& Success)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		Success = World->GetSubsystem<UDueTweenSubsystem>()->ResumeTween(DueTweenHandle);
	}
	Success = false;
}

void UDueTweenBlueprintFunctionLibrary::FastForwardDueTween(UObject* Target, const int& DueTweenHandle, bool& Success)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		Success = World->GetSubsystem<UDueTweenSubsystem>()->FastForwardTween(DueTweenHandle);
	}
	Success = false;
}

void UDueTweenBlueprintFunctionLibrary::StopDueTween(UObject* Target, const int& DueTweenHandle, bool& Success)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(Target, EGetWorldErrorMode::ReturnNull))
	{
		Success = World->GetSubsystem<UDueTweenSubsystem>()->StopTween(DueTweenHandle);
	}
	Success = false;
}

FActiveDueTweenHandle UDueTweenBlueprintFunctionLibrary::CreateAndStartLatentAction(
	UWorld* World, const FLatentActionInfo& LatentInfo,
	const FDUETweenData& TweenData)
{
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
		nullptr)
	{
		const FActiveDueTweenHandle NewHandle = World->GetSubsystem<UDueTweenSubsystem>()->AddTween(TweenData);
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
		                                 new FDueTweenAction(LatentInfo, NewHandle));
		UE_LOG(LogDUETween, Verbose, TEXT("Starting latent due tween action with UUID: %d"), LatentInfo.UUID);
		return NewHandle;
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unable to start due tween latent action with UUID: %d"), LatentInfo.UUID);
	return NULL_DUETWEEN_HANDLE;
}
