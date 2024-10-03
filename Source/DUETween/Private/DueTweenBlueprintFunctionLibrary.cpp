// Fill out your copyright notice in the Description page of Project Settings.


#include "DueTweenBlueprintFunctionLibrary.h"

#include "DUETween.h"
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
		FActiveDueTweenHandle Handle = NULL_DUETWEEN_HANDLE;
		if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
			TargetAsSceneComponent)
		{
			FVector StartingValue = TargetAsSceneComponent->GetRelativeLocation();
			FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
			                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
				SceneComp->SetRelativeLocation(UpdatedValue.GetSubtype<FVector>());
			};
			Handle = DUETween::StartTween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
			                              DueEasingType, Steps);
		}
		else if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
		{
			FVector StartingValue = TargetAsActor->GetActorLocation();
			FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
			                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
				TargetActor->SetActorLocation(UpdatedValue.GetSubtype<FVector>());
			};
			Handle = DUETween::StartTween(Target, TargetCallback, StartingValue, TargetLocation, Duration,
			                              DueEasingType, Steps);
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
		FActiveDueTweenHandle Handle = NULL_DUETWEEN_HANDLE;

		if (UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(Target);
			TargetAsCanvasPanelSlot)
		{
			FVector2D StartingValue = TargetAsCanvasPanelSlot->GetPosition();
			FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
			                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(TargetToUpdate.Get());
				CanvasPanelSlot->SetPosition(UpdatedValue.GetSubtype<FVector2D>());
			};
			Handle = DUETween::StartTween(Target, TargetCallback, StartingValue, TargetValue, Duration,
			                              DueEasingType, Steps);
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due move 2d: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
		FActiveDueTweenHandle Handle = NULL_DUETWEEN_HANDLE;

		if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(Target);
			TargetAsSceneComponent)
		{
			FRotator StartingValue = TargetAsSceneComponent->GetRelativeRotation();
			FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
			                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				USceneComponent* SceneComp = Cast<USceneComponent>(TargetToUpdate.Get());
				SceneComp->SetRelativeRotation(UpdatedValue.GetSubtype<FRotator>());
			};
			Handle = DUETween::StartTween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
			                              DueEasingType, Steps);
		}
		else if (AActor* TargetAsActor = Cast<AActor>(Target); TargetAsActor)
		{
			FRotator StartingValue = TargetAsActor->GetActorRotation();
			FTweenCallback TargetCallback = [](const FValueContainer& UpdatedValue,
			                                   const TWeakObjectPtr<UObject>& TargetToUpdate)
			{
				AActor* TargetActor = Cast<AActor>(TargetToUpdate.Get());
				TargetActor->SetActorRotation(UpdatedValue.GetSubtype<FRotator>());
			};
			Handle = DUETween::StartTween(Target, TargetCallback, StartingValue, TargetRotation, Duration,
			                              DueEasingType, Steps);
		}
		else
		{
			UE_LOG(LogDUETween, Error, TEXT("Unsupported type for due rotate: %s"),
			       *Target->StaticClass()->GetClassPathName().ToString());
		}


		OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
			FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (PropertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			const FActiveDueTweenHandle Handle = DUETween::StartTween(Target, PropertyRef, TargetValue, Duration,
			                                                          DueEasingType, Steps);

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
			FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (PropertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			const FActiveDueTweenHandle Handle = DUETween::StartTween(Target, PropertyRef, TargetValue, Duration,
																	  DueEasingType, Steps);

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
			FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (PropertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			const FActiveDueTweenHandle Handle = DUETween::StartTween(Target, PropertyRef, TargetValue, Duration,
																	  DueEasingType, Steps);

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
		FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(FieldName);
		if (PropertyRef == nullptr)
		{
			UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
			return;
		}
		const FActiveDueTweenHandle Handle = DUETween::StartTween(Target, PropertyRef, TargetValue, Duration,
																  DueEasingType, Steps);

		OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
			FProperty* PropertyRef = Target->GetClass()->FindPropertyByName(FieldName);
			if (PropertyRef == nullptr)
			{
				UE_LOG(LogDUETween, Error, TEXT("No Property Found For:%s"), *FieldName.ToString());
				return;
			}
			const FActiveDueTweenHandle Handle = DUETween::StartTween(Target, PropertyRef, TargetValue, Duration,
																	  DueEasingType, Steps);

			OutHandle = CreateAndStartLatentAction(World, LatentInfo, Handle);
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
	const FActiveDueTweenHandle& TweenHandle)
{
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	if (LatentActionManager.FindExistingAction<FDueTweenAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) ==
		nullptr || TweenHandle == NULL_DUETWEEN_HANDLE)
	{
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,
		                                 new FDueTweenAction(LatentInfo, TweenHandle));
		UE_LOG(LogDUETween, Verbose, TEXT("Starting latent due tween action with UUID: %d"), LatentInfo.UUID);
		return TweenHandle;
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unable to start due tween latent action with UUID: %d"), LatentInfo.UUID);
	return NULL_DUETWEEN_HANDLE;
}
