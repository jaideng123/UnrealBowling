#include "TweenFunctions.h"

#include "DUETween.h"

double TweenFunctions::Ease(double A, double B, double Alpha, TEnumAsByte<EasingType> Easing, double BlendExp,
                            int32 Steps)
{
	// Clamp Alpha In-case it exceeds 1 or is negative
	Alpha = FMath::Clamp(Alpha, 0.0, 1.0);

	switch (Easing)
	{
	case EasingType::Linear:
		return LinearEase(A, B, Alpha);
		break;
	case EasingType::Step:
		return LinearStepEase(A, B, Alpha, Steps);
		break;
	case EasingType::SinusoidalIn:
		return EaseInSin(A, B, Alpha);
		break;
	case EasingType::SinusoidalOut:
		break;
	case EasingType::SinusoidalInOut:
		break;
	case EasingType::EaseIn:
		break;
	case EasingType::EaseOut:
		break;
	case EasingType::EaseInOut:
		break;
	case EasingType::ExpoIn:
		break;
	case EasingType::ExpoOut:
		break;
	case EasingType::ExpoInOut:
		break;
	case EasingType::CircularIn:
		break;
	case EasingType::CircularOut:
		break;
	case EasingType::CircularInOut:
		break;
	}

	UE_LOG(LogDUETween, Warning, TEXT("Undefined Method For Easing: %s"),*UEnum::GetValueAsString<EasingType>(Easing));
	return LinearEase(A, B, Alpha);
}

double TweenFunctions::LinearEase(double A, double B, double Alpha)
{
	return A + ((B - A) * Alpha);
}

double TweenFunctions::LinearStepEase(double A, double B, double Alpha, int32 Steps)
{
	const double StepsAsDouble = Steps;
	const double NumIntervals = StepsAsDouble - 1.0;
	double const ModifiedAlpha = FMath::Floor(Alpha * StepsAsDouble) / NumIntervals;
	return LinearEase(A, B, ModifiedAlpha);
}

double TweenFunctions::EaseInSin(double A, double B, double Alpha)
{
	double ModifiedAlpha = 1 - FMath::Cos(Alpha * UE_DOUBLE_HALF_PI);
	return LinearEase(A, B, ModifiedAlpha);
}
