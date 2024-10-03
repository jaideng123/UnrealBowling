#pragma once
#include <functional>

#include "DueEasingFunctionLibrary.h"
#include "Containers/Union.h"

typedef int FActiveDueTweenHandle;
constexpr FActiveDueTweenHandle NULL_DUETWEEN_HANDLE = -1;

using FValueContainer = TUnion<FVector, FVector2D, FRotator, float, double>;
using FTweenCallback = TUniqueFunction<void(const FValueContainer&, const TWeakObjectPtr<UObject>&)>;

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
	Unset,
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
	 * 6. GetDueValueType
	 */
};

// This doesn't work if you change or add anything for some reason
// TODO: figure out why
template <typename T>
constexpr EDueValueType GetDueValueTypeConst()
{
	if (std::is_same_v<T, const float&>)
	{
		return EDueValueType::Float;
	}
	if (std::is_same_v<T, const double&>)
	{
		return EDueValueType::Double;
	}
	if (std::is_same_v<T, const FVector&>)
	{
		return EDueValueType::Vector;
	}
	if (std::is_same_v<T, const FRotator&>)
	{
		return EDueValueType::Rotator;
	}
	if (std::is_same_v<T, const FVector2D&>)
	{
		return EDueValueType::Vector2D;
	}
	return EDueValueType::Unset;
}


// This is necessary because above only works as constexpr and this doesnt
template <typename T>
constexpr EDueValueType GetDueValueType()
{
	if (std::is_same_v<T, float>)
	{
		return EDueValueType::Float;
	}
	if (std::is_same_v<T, double>)
	{
		return EDueValueType::Double;
	}
	if (std::is_same_v<T, FVector>)
	{
		return EDueValueType::Vector;
	}
	if (std::is_same_v<T, FRotator>)
	{
		return EDueValueType::Rotator;
	}
	if (std::is_same_v<T, FVector2D>)
	{
		return EDueValueType::Vector2D;
	}
	return EDueValueType::Unset;
}

/**
 * Value type for data manipulated by tween
 */
enum class EDueUpdateType
{
	Unset,
	Property,
	Function
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
	FTweenCallback TargetCallback;
	FProperty* TargetProperty;

	EDueUpdateType UpdateType = EDueUpdateType::Unset;

	FValueContainer TargetValue;
	FValueContainer StartingValue;
	EDueValueType ValueType = EDueValueType::Float;

	TWeakObjectPtr<UObject> Target;
	float Duration = 0;
	EDueEasingType EasingType = EDueEasingType::Linear;
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
