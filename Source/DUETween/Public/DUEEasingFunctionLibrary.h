#pragma once
#include "Kismet/KismetMathLibrary.h"

// Easing Types mapping to various functions
// Taken from https://easings.net/
UENUM()
enum EDueEasingType : int
{
	// Linear Interp
	Linear,
	// Sinusoidal Interps
	InSin,
	OutSin,
	InOutSin,
	// Quad (x^2) Interps
	InQuad,
	OutQuad,
	InOutQuad,
	// Cubic (x^3) Interps
	InCubic,
	OutCubic,
	InOutCubic,
	// Quart (x^4) Interps
	InQuart,
	OutQuart,
	InOutQuart,
	// Quint (x^5) Interps
	InQuint,
	OutQuint,
	InOutQuint,
	// Exponential (2^x) Interps
	InExpo,
	OutExpo,
	InOutExpo,
	// Circular (sqrt) Interps
	InCircular,
	OutCircular,
	InOutCircular,
	// Elastic Interps
	InElastic,
	OutElastic,
	InOutElastic,
	// In Back Interps
	InBack,
	OutBack,
	InOutBack,
	// Bounce Interps
	InBounce,
	OutBounce,
	InOutBounce
};

class DUEEasingFunctionLibrary
{
public:
	static double Ease(double A, double B, double Alpha, EDueEasingType EasingType, int32 Steps = 0);

	// Easing Functions
	static double EaseInSin(double Alpha);
	static double EaseOutSin(double Alpha);
	static double EaseInOutSin(double Alpha);
	static double EaseInQuad(double Alpha);
	static double EaseOutQuad(double Alpha);
	static double EaseInOutQuad(double Alpha);
	static double EaseInCubic(double Alpha);
	static double EaseOutCubic(double Alpha);
	static double EaseInOutCubic(double Alpha);
	static double EaseInQuart(double Alpha);
	static double EaseOutQuart(double Alpha);
	static double EaseInOutQuart(double Alpha);
	static double EaseInQuint(double Alpha);
	static double EaseOutQuint(double Alpha);
	static double EaseInOutQuint(double Alpha);
	static double EaseInExpo(double Alpha);
	static double EaseOutExpo(double Alpha);
	static double EaseInOutExpo(double Alpha);
	static double EaseInCirc(double Alpha);
	static double EaseOutCirc(double Alpha);
	static double EaseInOutCirc(double Alpha);
	static double EaseInElastic(double Alpha);
	static double EaseOutElastic(double Alpha);
	static double EaseInOutElastic(double Alpha);
	static double EaseInBack(double Alpha);
	static double EaseOutBack(double Alpha);
	static double EaseInOutBack(double Alpha);
	static double EaseInBounce(double Alpha);
	static double EaseOutBounce(double Alpha);
	static double EaseInOutBounce(double Alpha);
};
