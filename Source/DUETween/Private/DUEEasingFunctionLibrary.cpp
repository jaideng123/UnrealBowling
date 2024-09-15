#include "DUEEasingFunctionLibrary.h"

#include "DUETween.h"
#include "Kismet/KismetMathLibrary.h"

double DUEEasingFunctionLibrary::Ease(double A, double B, double Alpha, DUEEasingType Easing, double BlendExp,
                                      int32 Steps)
{
	// Clamp Alpha In-case it exceeds 1 or is negative
	Alpha = FMath::Clamp(Alpha, 0.0, 1.0);

	switch (Easing)
	{
	case DUEEasingType::Linear:
		return LinearEase(A, B, Alpha);
		break;
	case DUEEasingType::Step:
		return LinearStepEase(A, B, Alpha, Steps);
		break;
	case DUEEasingType::InSin:
		return EaseInSin(A, B, Alpha);
		break;
		//TODO fill these out
	case DUEEasingType::OutSin:
		break;
	case DUEEasingType::InOutSin:
		break;
	case DUEEasingType::EaseIn:
		break;
	case DUEEasingType::EaseOut:
		break;
	case DUEEasingType::EaseInOut:
		break;
	case DUEEasingType::ExpoIn:
		break;
	case DUEEasingType::ExpoOut:
		break;
	case DUEEasingType::ExpoInOut:
		break;
	case DUEEasingType::CircularIn:
		break;
	case DUEEasingType::CircularOut:
		break;
	case DUEEasingType::CircularInOut:
		break;
	}

	UE_LOG(LogDUETween, Warning, TEXT("Undefined Method For Easing: %s using Linear"),
	       *UEnum::GetValueAsString<DUEEasingType>(Easing));
	return LinearEase(A, B, Alpha);
}

double DUEEasingFunctionLibrary::LinearEase(double A, double B, double Alpha)
{
	return A + ((B - A) * Alpha);
}

double DUEEasingFunctionLibrary::LinearStepEase(double A, double B, double Alpha, int32 Steps)
{
	const double StepsAsDouble = Steps;
	const double NumIntervals = StepsAsDouble - 1.0;
	double const ModifiedAlpha = FMath::Floor(Alpha * StepsAsDouble) / NumIntervals;
	return LinearEase(A, B, ModifiedAlpha);
}

double DUEEasingFunctionLibrary::EaseInSin(double A, double B, double Alpha)
{
	double ModifiedAlpha = 1 - FMath::Cos(Alpha * UE_DOUBLE_HALF_PI);
	return LinearEase(A, B, ModifiedAlpha);
}

DUEEasingType DUEEasingFunctionLibrary::ConvertFromUnrealEasingType(EEasingFunc::Type unrealEasing)
{
	switch (unrealEasing)
	{
	case EEasingFunc::Linear:
		return DUEEasingType::Linear;
		break;
	case EEasingFunc::Step:
		return DUEEasingType::Step;
		break;
	case EEasingFunc::SinusoidalIn:
		return DUEEasingType::InSin;
		break;
	case EEasingFunc::SinusoidalOut:
		return DUEEasingType::OutSin;
		break;
	case EEasingFunc::SinusoidalInOut:
		return DUEEasingType::InOutSin;
		break;
	case EEasingFunc::EaseIn:
		return DUEEasingType::EaseIn;
		break;
	case EEasingFunc::EaseOut:
		return DUEEasingType::EaseOut;
		break;
	case EEasingFunc::EaseInOut:
		return DUEEasingType::EaseInOut;
		break;
	case EEasingFunc::ExpoIn:
		return DUEEasingType::ExpoIn;
		break;
	case EEasingFunc::ExpoOut:
		return DUEEasingType::ExpoOut;
		break;
	case EEasingFunc::ExpoInOut:
		return DUEEasingType::ExpoInOut;
		break;
	case EEasingFunc::CircularIn:
		return DUEEasingType::CircularIn;
		break;
	case EEasingFunc::CircularOut:
		return DUEEasingType::CircularOut;
		break;
	case EEasingFunc::CircularInOut:
		return DUEEasingType::CircularInOut;
		break;
	}
	UE_LOG(LogDUETween, Warning, TEXT("Unmapped Type For Unreal Easing: %s returning Linear"),
	       *UEnum::GetValueAsString<EEasingFunc::Type>(unrealEasing));
	return DUEEasingType::Linear;
}
