#pragma once
#include <functional>

#include "DueEasingFunctionLibrary.h"
#include "Containers/Union.h"

typedef int FActiveDueTweenHandle;
constexpr FActiveDueTweenHandle NULL_DUETWEEN_HANDLE = -1;

using FValueContainer = TUnion<FVector, FVector2D, FRotator, float, double>;

/**
 * Status of a tween
 */
enum class EDueTweenStatus
{
	// This tween is unused and ready to be reclaimed
	Unset,
	// This tween is running
	Running,
	// This tween is completed and will be marked unused next frame
	Completed,
	// This tween is paused
	Paused,
	// This tween is being fast-forwarded to it's end point
	FastForward,
};

/**
 * Value type for data manipulated by tween
 */
enum class EDueValueType
{
	Float,
	Double,
	Vector,
	Rotator,
	Vector2D
	/**
	 * When adding a new type update:
	 * 1. This
	 * 2. FValueContainer
	 * 3. Set and GetCurrentValueFromProperty
	 * 4. TickTween
	 * 5. UDueTweenBlueprintFunctionLibrary
	 */
};

/**
 * Value type for data manipulated by tween
 */
enum class EDueUpdateType
{
	Unset,
	Property
	/**
	 * When adding a new type update:
	 * 1. This
	 * 2. FValueContainer
	 * 3. Set and GetCurrentValueFromProperty
	 * 4. TickTween
	 * 5. UDueTweenBlueprintFunctionLibrary
	 */
};

/**
 * Data that defines the characteristics of the tween
 */
struct FDUETweenData
{
	union
	{
		TFunction<void(const FValueContainer&, const TWeakObjectPtr<UObject>&)> TargetCallback;
		FProperty* TargetProperty;
	} UpdateData;

	EDueUpdateType UpdateType = EDueUpdateType::Unset;

	TWeakObjectPtr<UObject> Target;
	FValueContainer TargetValue;
	FValueContainer StartingValue;
	float Duration = 0;
	EDueEasingType EasingType = EDueEasingType::Linear;
	EDueValueType ValueType = EDueValueType::Float;
	int32 Steps = 0;
};

/**
 * Data structure for an active tween
 */
struct FActiveDueTween
{
	union
	{
		struct FActiveNode
		{
			FActiveDueTweenHandle NextActiveTween;
			FActiveDueTweenHandle LastActiveTween;
		};

		FActiveNode ActiveNode;
		FActiveDueTweenHandle NextFreeTween;
	} TweenPtr;

	FDUETweenData TweenData;

	// Constant state
	FActiveDueTweenHandle Handle;

	// Per-Tween State
	unsigned int ID = 0;
	float TimeElapsed = 0;
	EDueTweenStatus Status = EDueTweenStatus::Unset;
	FValueContainer StartingValue;
};

static_assert(std::is_trivially_copyable_v<EDueTweenStatus> == true);