#pragma once
#include "DueEasingFunctionLibrary.h"
#include "Containers/Union.h"

typedef int FActiveDueTweenHandle;
constexpr FActiveDueTweenHandle NULL_DUETWEEN_HANDLE = -1;

using FValueContainer = TUnion<FVector, FRotator, float, double>;

enum class EDUETweenStatus
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

enum class EDUEValueType
{
	Float,
	Double,
	Vector,
	Rotator,
};

// Data that defines the characteristics of the tween
struct FDUETweenData
{
	FProperty* TargetProperty;
	TWeakObjectPtr<> Target;
	FValueContainer TargetValue;
	float Duration = 0;
	EDueEasingType EasingType = EDueEasingType::Linear;
	EDUEValueType ValueType = EDUEValueType::Float;
	int32 Steps = 0;
};

struct FActiveDueTween
{
	FDUETweenData TweenData;

	// Constant state
	FActiveDueTweenHandle Handle;

	// Per-Tween State
	unsigned int ID = 0;
	float TimeElapsed = 0;
	EDUETweenStatus Status = EDUETweenStatus::Unset;
	FValueContainer StartingValue;

	union
	{
		FActiveDueTweenHandle NextFreeTween;

		struct FActiveNode
		{
			FActiveDueTweenHandle NextActiveTween;
			FActiveDueTweenHandle LastActiveTween;
		};

		FActiveNode ActiveNode;
	} TweenPtr;
};
