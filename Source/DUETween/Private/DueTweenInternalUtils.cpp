#include "DueTweenInternalUtils.h"

#include "Components/CanvasPanelSlot.h"


FValueContainer FDueTweenInternalUtils::GetCurrentValueFromProperty(const FDUETweenData& TweenData)
{
	if (!TweenData.Target.IsValid())
	{
		return FValueContainer();
	}
	switch (TweenData.ValueType)
	{
	case EDueValueType::Float:
		{
			if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty))
			{
				float StartingValue;
				FloatProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDueValueType::Double:
		{
			if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty))
			{
				double StartingValue;
				DoubleProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDueValueType::Vector:
		{
			// We interpret null property as actor/component location
			if (TweenData.TargetProperty == nullptr)
			{
				if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					return FValueContainer(TargetAsSceneComponent->GetRelativeLocation());
				}
				if (const AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					return FValueContainer(TargetAsActor->GetActorLocation());
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
				return FValueContainer();
			}
			if (FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty); VectorProperty
				&& VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				const FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TweenData.Target.Get());
				const FVector StartingValue = *StructAddress;

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDueValueType::Rotator:
		{
			// We interpret null property as actor/component rotation
			if (TweenData.TargetProperty == nullptr)
			{
				if (const USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					return FValueContainer(TargetAsSceneComponent->GetRelativeRotation());
				}
				if (const AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					return FValueContainer(TargetAsActor->GetActorRotation());
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
				return FValueContainer();
			}
			if (const FStructProperty* RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				RotatorProperty
				&& RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				const FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(
					TweenData.Target.Get());
				const FRotator StartingValue = *StructAddress;

				return FValueContainer(StartingValue);
			}
			break;
		}
	case EDueValueType::Vector2D:
		// We interpret null property as actor/component rotation
		if (TweenData.TargetProperty == nullptr)
		{
			if (const UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(TweenData.Target.Get());
				TargetAsCanvasPanelSlot)
			{
				return FValueContainer(TargetAsCanvasPanelSlot->GetPosition());
			}
			UE_LOG(LogDUETween, Error,
			       TEXT("Unable to support null target property on type: %s"),
			       *TweenData.Target.Get()->GetClass()->GetName());
			return FValueContainer();
		}
		if (FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty); VectorProperty
			&& VectorProperty->Struct == TBaseStructure<FVector2D>::Get())
		{
			const FVector2D* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector2D>(TweenData.Target.Get());
			const FVector2D StartingValue = *StructAddress;

			return FValueContainer(StartingValue);
		}
		break;
	}
	UE_LOG(LogDUETween, Error,
	       TEXT("Unsupported Get for Value Type: %d"),
	       TweenData.ValueType);
	return FValueContainer();
}

void FDueTweenInternalUtils::SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue)
{
	DECLARE_CYCLE_STAT(TEXT("SetCurrentValue"), STAT_SetCurrentValue, STATGROUP_DUETWEEN);
	SCOPE_CYCLE_COUNTER(STAT_SetCurrentValue);

	if (!TweenData.Target.IsValid())
	{
		return;
	}
	// TODO check type
	if(TweenData.TargetCallback)
	{
		TweenData.TargetCallback(NewValue, TweenData);
	}
	if (TweenData.TargetProperty == nullptr)
	{
		return;
	}
	
	switch (TweenData.ValueType)
	{
	case EDueValueType::Float:
		{
			if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(TweenData.TargetProperty))
			{
				FloatProperty->SetValue_InContainer(TweenData.Target.Get(), NewValue.GetSubtype<float>());
			}
			break;
		}
	case EDueValueType::Double:
		{
			if (const FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(TweenData.TargetProperty))
			{
				DoubleProperty->SetValue_InContainer(TweenData.Target.Get(), NewValue.GetSubtype<double>());
			}
			break;
		}
	case EDueValueType::Vector:
		{
			const FVector NewVector = NewValue.GetSubtype<FVector>();
			UObject* TargetUObject = TweenData.Target.Get();
			// We interpret null property as actor/component location
			if (TweenData.TargetProperty == nullptr)
			{
				DECLARE_CYCLE_STAT(TEXT("SetCurrentValue_Cast"), STAT_SetCurrentValue_Cast, STATGROUP_DUETWEEN);
				SCOPE_CYCLE_COUNTER(STAT_SetCurrentValue_Cast);
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TargetUObject);
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeLocation(NewVector);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TargetUObject); TargetAsActor)
				{
					TargetAsActor->SetActorLocation(NewVector);
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null TargetUObject property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}

			if (const FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				VectorProperty
				&& VectorProperty->Struct == TBaseStructure<FVector>::Get())
			{
				FVector* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector>(TargetUObject);
				*StructAddress = NewVector;
			}
			break;
		}
	case EDueValueType::Rotator:
		{
			FRotator NewRotatorValue = NewValue.GetSubtype<FRotator>();
			// We interpret null property as actor/component rotation
			if (TweenData.TargetProperty == nullptr)
			{
				if (USceneComponent* TargetAsSceneComponent = Cast<USceneComponent>(TweenData.Target.Get());
					TargetAsSceneComponent)
				{
					TargetAsSceneComponent->SetRelativeRotation(NewRotatorValue);
					return;
				}
				if (AActor* TargetAsActor = Cast<AActor>(TweenData.Target.Get()); TargetAsActor)
				{
					TargetAsActor->SetActorRotation(NewRotatorValue);
					return;
				}
				UE_LOG(LogDUETween, Error,
				       TEXT("Unable to support null target property on non component/actor type: %s"),
				       *TweenData.Target.Get()->GetClass()->GetName());
			}
			if (const FStructProperty* RotatorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
				RotatorProperty
				&& RotatorProperty->Struct == TBaseStructure<FRotator>::Get())
			{
				FRotator* StructAddress = RotatorProperty->ContainerPtrToValuePtr<FRotator>(TweenData.Target.Get());
				*StructAddress = NewRotatorValue;
			}
			break;
		}
	case EDueValueType::Vector2D:
		if (TweenData.TargetProperty == nullptr)
		{
			if (UCanvasPanelSlot* TargetAsCanvasPanelSlot = Cast<UCanvasPanelSlot>(TweenData.Target.Get());
				TargetAsCanvasPanelSlot)
			{
				TargetAsCanvasPanelSlot->SetPosition(NewValue.GetSubtype<FVector2D>());
			}
			UE_LOG(LogDUETween, Error,
			       TEXT("Unable to support null target property on type: %s"),
			       *TweenData.Target.Get()->GetClass()->GetName());
			return;
		}
		if (const FStructProperty* VectorProperty = CastField<FStructProperty>(TweenData.TargetProperty);
			VectorProperty
			&& VectorProperty->Struct == TBaseStructure<FVector2D>::Get())
		{
			FVector2D* StructAddress = VectorProperty->ContainerPtrToValuePtr<FVector2D>(TweenData.Target.Get());
			*StructAddress = NewValue.GetSubtype<FVector2D>();
		}
		break;
	}
}