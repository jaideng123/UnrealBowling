#pragma once
#include "Kismet/KismetMathLibrary.h"



// DoTweenEasings for Ref:
// public enum Ease
//   {
//     Unset,
//     Linear,
//     InSine,
//     OutSine,
//     InOutSine,
//     InQuad,
//     OutQuad,
//     InOutQuad,
//     InCubic,
//     OutCubic,
//     InOutCubic,
//     InQuart,
//     OutQuart,
//     InOutQuart,
//     InQuint,
//     OutQuint,
//     InOutQuint,
//     InExpo,
//     OutExpo,
//     InOutExpo,
//     InCirc,
//     OutCirc,
//     InOutCirc,
//     InElastic,
//     OutElastic,
//     InOutElastic,
//     InBack,
//     OutBack,
//     InOutBack,
//     InBounce,
//     OutBounce,
//     InOutBounce,
//     Flash,
//     InFlash,
//     OutFlash,
//     InOutFlash,
//     /// <summary>
//     /// Don't assign this! It's assigned automatically when creating 0 duration tweens
//     /// </summary>
//     INTERNAL_Zero,
//     /// <summary>
//     /// Don't assign this! It's assigned automatically when setting the ease to an AnimationCurve or to a custom ease function
//     /// </summary>
//     INTERNAL_Custom,
//   }

// Easing Types mapping to various functions
// Taken from https://easings.net/
UENUM()
enum class DUEEasingType : int {
	/** Simple linear interpolation. */
	Linear,

	/** Simple step interpolation. */
	Step,

	/** Sinusoidal in interpolation. */
	InSin,

	/** Sinusoidal out interpolation. */
	OutSin,

	/** Sinusoidal in/out interpolation. */
	InOutSin,

	/** Smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by BlendExp. */
	EaseIn,

	/** Immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by BlendExp. */
	EaseOut,

	/** Smoothly accelerates and decelerates.  Ease amount controlled by BlendExp. */
	EaseInOut,

	/** Easing in using an exponential */
	ExpoIn,

	/** Easing out using an exponential */
	ExpoOut,

	/** Easing in/out using an exponential method */
	ExpoInOut,

	/** Easing is based on a half circle. */
	CircularIn,

	/** Easing is based on an inverted half circle. */
	CircularOut,

	/** Easing is based on two half circles. */
	CircularInOut
};

class DUEEasingFunctionLibrary
{
public:
	// TODO: Handle Vectors, Rotators, Etc.
	static double Ease(double A, double B, double Alpha, DUEEasingType EasingType, double BlendExp = 2, int32 Steps = 2);
	static double LinearEase(double X, double B, double Alpha);
	static double LinearStepEase(double X, double B, double Alpha, int32 INT32);
	static double EaseInSin(double X, double B, double Alpha);
	static DUEEasingType ConvertFromUnrealEasingType(EEasingFunc::Type unrealEasing);

};
