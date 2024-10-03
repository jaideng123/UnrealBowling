#pragma once
#include "ActiveDueTween.h"
#include "DueTweenSubsystem.h"

class DUETWEEN_API DUETween
{
public:
	template <typename T>
	static FActiveDueTweenHandle StartTween(const TWeakObjectPtr<UObject> Target,
	                                        FProperty* Property,
	                                        const T& TargetValue,
	                                        const float& Duration,
	                                        const EDueEasingType& Easing = EDueEasingType::InOutSin,
	                                        const int32& Steps = 0)
	{
		static_assert(GetDueValueTypeConst<T>() == EDueValueType::Unset, "Unsupported Type");

		FDUETweenData TweenData;
		TweenData.Target = Target;
		TweenData.Duration = Duration;
		TweenData.EasingType = Easing;
		TweenData.Steps = Steps;
		TweenData.UpdateType = EDueUpdateType::Property;
		TweenData.TargetProperty = Property;
		TweenData.TargetValue.SetSubtype<T>(TargetValue);
		TweenData.ValueType = GetDueValueType<T>();

		const UWorld* World = Target.Get()->GetWorld();
		return World->GetSubsystem<UDueTweenSubsystem>()->AddTween(TweenData);
	}

	template <typename T>
	static FActiveDueTweenHandle StartTween(const TWeakObjectPtr<UObject> Target,
	                                        FTweenCallback& Callback,
	                                        const T& StartingValue,
	                                        const T& TargetValue,
	                                        const float& Duration,
	                                        const EDueEasingType& Easing = EDueEasingType::InOutSin,
	                                        const int32& Steps = 0)
	{
		static_assert(GetDueValueTypeConst<T>() == EDueValueType::Unset, "Unsupported Type");

		FDUETweenData TweenData;
		TweenData.Target = Target;
		TweenData.Duration = Duration;
		TweenData.EasingType = Easing;
		TweenData.Steps = Steps;
		TweenData.UpdateType = EDueUpdateType::Function;
		TweenData.TargetCallback = std::move(Callback);
		TweenData.StartingValue.SetSubtype<T>(StartingValue);
		TweenData.TargetValue.SetSubtype<T>(TargetValue);
		TweenData.ValueType = GetDueValueType<T>();

		const UWorld* World = Target.Get()->GetWorld();
		return World->GetSubsystem<UDueTweenSubsystem>()->AddTween(TweenData);
	}
};
