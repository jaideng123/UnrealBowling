// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenBlueprintFunctionLibrary.h"

#include "DueTweenSubsystem.h"
#include "DueTweenAction.h"
#include "Components/CanvasPanelSlot.h"

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
		FDUETweenData TweenData;
		TweenData.Target = Target;
		TweenData.Duration = Duration;
		TweenData.EasingType = DueEasingType;
		TweenData.Steps = Steps;
		TweenData.TargetValue.SetSubtype<FVector>(TargetLocation);
		TweenData.ValueType = EDueValueType::Vector;
		TWeakObjectPtr<UObject> TargetWeakPtr = TweenData.Target;
		TweenData.UpdateType = EDueUpdateType::Function;
		if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
			TargetAsSceneComponent)
		{
			TweenData.StartingValue.SetSubtype<FVector>(TargetAsSceneComponent->GetRelativeLocation());
			TweenData.TargetCallback = [](const FValueContainer& UpdatedValue,
			                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
				SceneComp->SetRelativeLocation(UpdatedValue.GetSubtype<FVector>());
			};
		}
		else if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
		{
			TweenData.StartingValue.SetSubtype<FVector>(TargetAsActor->GetActorLocation());
			TweenData.TargetCallback = [](const FValueContainer& UpdatedValue,
			                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
				TargetActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
			};
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, TweenData);
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
		FDUETweenData tweenData;
		tweenData.Target = Target;
		tweenData.Duration = Duration;
		tweenData.EasingType = DueEasingType;
		tweenData.Steps = Steps;
		tweenData.TargetValue.SetSubtype<FVector2D>(TargetValue);
		tweenData.ValueType = EDueValueType::Vector2D;

		tweenData.UpdateType = EDueUpdateType::Function;
		if (UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(tweenData.Target.Get());
			TargetAsCanvasPanelSlot)
		{
			tweenData.StartingValue.SetSubtype<FVector2D>(TargetAsCanvasPanelSlot->GetPosition());
			tweenData.TargetCallback = [](const FValueContainer& UpdatedValue,
			                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(TargetToUpdate.Get());
				CanvasPanelSlot->SetPosition(UpdatedValue.GetSubtype<FVector2D>());
			};
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move 2d: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}

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
		FDUETweenData tweenData;
		tweenData.Target = Target;
		tweenData.Duration = Duration;
		tweenData.EasingType = DueEasingType;
		tweenData.Steps = Steps;
		tweenData.TargetValue.SetSubtype<FRotator>(TargetRotation);
		tweenData.ValueType = EDueValueType::Rotator;
		tweenData.UpdateType = EDueUpdateType::Function;
		if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
			TargetAsSceneComponent)
		{
			tweenData.StartingValue.SetSubtype<FRotator>(TargetAsSceneComponent->GetRelativeRotation());
			tweenData.TargetCallback = [](const FValueContainer& UpdatedValue,
			                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
				SceneComp->SetRelativeRotation(UpdatedValue.GetSubtype<FRotator>());
			};
		}
		else if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
		{
			tweenData.StartingValue.SetSubtype<FRotator>(TargetAsActor->GetActorRotation());
			tweenData.TargetCallback = [](const FValueContainer& UpdatedValue,
			                                         const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
				TargetActor->SetActorRotation(UpdatedValue.GetSubtype<FRotator>());
			};
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}


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
			tweenData.UpdateType = EDueUpdateType::Property;
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
			tweenData.UpdateType = EDueUpdateType::Property;
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
			tweenData.UpdateType = EDueUpdateType::Property;
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
			tweenData.UpdateType = EDueUpdateType::Property;
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
			tweenData.UpdateType = EDueUpdateType::Property;
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
	FDUETweenData& TweenData)
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
