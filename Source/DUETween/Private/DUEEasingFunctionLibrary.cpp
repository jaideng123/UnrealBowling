#include "DUEEasingFunctionLibrary.h"

#include "DUETween.h"
#include "Kismet/KismetMathLibrary.h"

double DUEEasingFunctionLibrary::Ease(double A, double B, double Alpha, EDueEasingType Easing, int32 Steps)
{
	// Clamp Input Alpha In-case it exceeds 1 or is negative
	Alpha = FMath::Clamp(Alpha, 0.0, 1.0);
	if (Steps > 0)
	{
		Alpha = FMath::InterpStep(0.0, 1.0, Alpha, Steps);
	}
	double ResultAlpha = Alpha;

	switch (Easing)
	{
	case EDueEasingType::Linear:
		// Linear case is skipped because it is just alpha
		break;
	case EDueEasingType::InSin:
		ResultAlpha = EaseInSin(Alpha);
		break;
	case EDueEasingType::OutSin:
		ResultAlpha = EaseOutSin(Alpha);
		break;
	case EDueEasingType::InOutSin:
		ResultAlpha = EaseInOutSin(Alpha);
		break;
	case EDueEasingType::InQuad:
		ResultAlpha = EaseInQuad(Alpha);
		break;
	case EDueEasingType::OutQuad:
		ResultAlpha = EaseOutQuad(Alpha);
		break;
	case EDueEasingType::InOutQuad:
		ResultAlpha = EaseInOutQuad(Alpha);
		break;
	case EDueEasingType::InCubic:
		ResultAlpha = EaseInCubic(Alpha);
		break;
	case EDueEasingType::OutCubic:
		ResultAlpha = EaseOutCubic(Alpha);
		break;
	case EDueEasingType::InOutCubic:
		ResultAlpha = EaseInOutCubic(Alpha);
		break;
	case EDueEasingType::InQuart:
		ResultAlpha = EaseInQuart(Alpha);
		break;
	case EDueEasingType::OutQuart:
		ResultAlpha = EaseOutQuart(Alpha);
		break;
	case EDueEasingType::InOutQuart:
		ResultAlpha = EaseInOutQuart(Alpha);
		break;
	case EDueEasingType::InQuint:
		ResultAlpha = EaseInQuint(Alpha);
		break;
	case EDueEasingType::OutQuint:
		ResultAlpha = EaseOutQuint(Alpha);
		break;
	case EDueEasingType::InOutQuint:
		ResultAlpha = EaseInOutQuint(Alpha);
		break;
	case EDueEasingType::InExpo:
		ResultAlpha = EaseInExpo(Alpha);
		break;
	case EDueEasingType::OutExpo:
		ResultAlpha = EaseOutExpo(Alpha);
		break;
	case EDueEasingType::InOutExpo:
		ResultAlpha = EaseInOutExpo(Alpha);
		break;
	case EDueEasingType::InCircular:
		ResultAlpha = EaseInCirc(Alpha);
		break;
	case EDueEasingType::OutCircular:
		ResultAlpha = EaseOutCirc(Alpha);
		break;
	case EDueEasingType::InOutCircular:
		ResultAlpha = EaseInOutCirc(Alpha);
		break;
	case EDueEasingType::InElastic:
		ResultAlpha = EaseInElastic(Alpha);
		break;
	case EDueEasingType::OutElastic:
		ResultAlpha = EaseOutElastic(Alpha);
		break;
	case EDueEasingType::InOutElastic:
		ResultAlpha = EaseInOutElastic(Alpha);
		break;
	case EDueEasingType::InBack:
		ResultAlpha = EaseInBack(Alpha);
		break;
	case EDueEasingType::OutBack:
		ResultAlpha = EaseOutBack(Alpha);
		break;
	case EDueEasingType::InOutBack:
		ResultAlpha = EaseInOutBack(Alpha);
		break;
	case EDueEasingType::InBounce:
		ResultAlpha = EaseInBounce(Alpha);
		break;
	case EDueEasingType::OutBounce:
		ResultAlpha = EaseOutBounce(Alpha);
		break;
	case EDueEasingType::InOutBounce:
		ResultAlpha = EaseInOutBounce(Alpha);
		break;
	default:
		UE_LOG(LogDUETween, Warning, TEXT("Undefined Method For Easing: %s using Linear"),
		       *UEnum::GetValueAsString<EDueEasingType>(Easing));
		break;
	}
	return FMath::Lerp(A, B, ResultAlpha);
}

double DUEEasingFunctionLibrary::EaseInSin(const double Alpha)
{
	return 1.0 - FMath::Cos(Alpha * UE_DOUBLE_HALF_PI);
}

double DUEEasingFunctionLibrary::EaseOutSin(const double Alpha)
{
	return FMath::Sin(Alpha * UE_DOUBLE_HALF_PI);
}

double DUEEasingFunctionLibrary::EaseInOutSin(const double Alpha)
{
	return -(FMath::Cos(UE_DOUBLE_PI * Alpha) - 1.0) / 2.0;
}

double DUEEasingFunctionLibrary::EaseInQuad(const double Alpha)
{
	return FMath::Square(Alpha);
}

double DUEEasingFunctionLibrary::EaseOutQuad(const double Alpha)
{
	return 1.0 - FMath::Square(1.0 - Alpha);
}

double DUEEasingFunctionLibrary::EaseInOutQuad(const double Alpha)
{
	return Alpha < 0.5 ? 2.0 * FMath::Square(Alpha) : 1.0 - (FMath::Pow(-2.0 * Alpha + 2.0, 2.0) / 2.0);
}

double DUEEasingFunctionLibrary::EaseInCubic(const double Alpha)
{
	return FMath::Cube(Alpha);
}

double DUEEasingFunctionLibrary::EaseOutCubic(const double Alpha)
{
	return 1.0 - FMath::Cube(1.0 - Alpha);
}

double DUEEasingFunctionLibrary::EaseInOutCubic(const double Alpha)
{
	return Alpha < 0.5 ? 4.0 * FMath::Cube(Alpha) : 1.0 - (FMath::Cube(-2.0 * Alpha + 2.0) / 2.0);
}

double DUEEasingFunctionLibrary::EaseInQuart(const double Alpha)
{
	return FMath::Pow(Alpha, 4.0);
}

double DUEEasingFunctionLibrary::EaseOutQuart(const double Alpha)
{
	return 1.0 - FMath::Pow(1.0 - Alpha, 4.0);
}

double DUEEasingFunctionLibrary::EaseInOutQuart(const double Alpha)
{
	return Alpha < 0.5 ? 8.0 * FMath::Pow(Alpha, 4.0) : 1.0 - (FMath::Pow(-2.0 * Alpha + 2.0, 4.0) / 2.0);
}

double DUEEasingFunctionLibrary::EaseInQuint(const double Alpha)
{
	return FMath::Pow(Alpha, 5.0);
}

double DUEEasingFunctionLibrary::EaseOutQuint(const double Alpha)
{
	return 1.0 - FMath::Pow(1.0 - Alpha, 5.0);
}

double DUEEasingFunctionLibrary::EaseInOutQuint(const double Alpha)
{
	return Alpha < 0.5 ? 16.0 * FMath::Pow(Alpha, 5.0) : 1.0 - (FMath::Pow(-2.0 * Alpha + 2.0, 5.0) / 2.0);
}

double DUEEasingFunctionLibrary::EaseInExpo(const double Alpha)
{
	return Alpha == 0.0 ? 0.0 : FMath::Pow(2.0, 10.0 * Alpha - 10.0);
}

double DUEEasingFunctionLibrary::EaseOutExpo(const double Alpha)
{
	return Alpha == 1.0 ? 1.0 : 1.0 - FMath::Pow(2.0, -10.0 * Alpha);
}

double DUEEasingFunctionLibrary::EaseInOutExpo(const double Alpha)
{
	if (Alpha == 0.0)
	{
		return 0;
	}
	if (Alpha == 1.0)
	{
		return 1;
	}
	return Alpha < 0.5
		       ? FMath::Pow(2.0, 20.0 * Alpha - 10.0) / 2.0
		       : (2.0 - FMath::Pow(2.0, -20.0 * Alpha + 10.0) / 2.0) / 2.0;
}

double DUEEasingFunctionLibrary::EaseInCirc(const double Alpha)
{
	return 1.0 - FMath::Sqrt(1.0 - FMath::Square(Alpha));
}

double DUEEasingFunctionLibrary::EaseOutCirc(const double Alpha)
{
	return FMath::Sqrt(1.0 - FMath::Square(Alpha - 1.0));
}

double DUEEasingFunctionLibrary::EaseInOutCirc(const double Alpha)
{
	return Alpha < 0.5
		       ? (1.0 - FMath::Sqrt(1.0 - FMath::Square(2.0 * Alpha))) / 2.0
		       : (FMath::Sqrt(1.0 - FMath::Square(-2.0 * Alpha + 2.0)) + 1.0) / 2.0;
}

double DUEEasingFunctionLibrary::EaseInElastic(const double Alpha)
{
	if (Alpha == 0)
	{
		return 0;
	}
	if (Alpha == 1)
	{
		return 1;
	}

	// TODO: allow configurations of amplitude + period
	constexpr double C4 = (2.0 * UE_DOUBLE_PI) / 3.0;
	return -FMath::Pow(2.0, 10.0 * Alpha - 10.0) * FMath::Sin((Alpha * 10.0 - 10.75) * C4);
}

double DUEEasingFunctionLibrary::EaseOutElastic(const double Alpha)
{
	if (Alpha == 0)
	{
		return 0;
	}
	if (Alpha == 1)
	{
		return 1;
	}

	// TODO: allow configurations of amplitude + period
	constexpr double C4 = (2.0 * UE_DOUBLE_PI) / 3.0;
	return FMath::Pow(2.0, -10.0 * Alpha) * FMath::Sin((Alpha * 10.0 - 0.75) * C4) + 1.0;
}

double DUEEasingFunctionLibrary::EaseInOutElastic(const double Alpha)
{
	if (Alpha == 0)
	{
		return 0;
	}
	if (Alpha == 1)
	{
		return 1;
	}

	// TODO: allow configurations of amplitude + period
	constexpr double C5 = (2.0 * UE_DOUBLE_PI) / 4.5;
	if (Alpha < 0.5)
	{
		return -(FMath::Pow(2.0, 20 * Alpha - 10) * FMath::Sin((Alpha * 20 - 11.125) * C5)) / 2.0;
	}
	return ((FMath::Pow(2.0, -20 * Alpha + 10) * FMath::Sin((Alpha * 20 - 11.125) * C5)) / 2) + 1.0;
}

double DUEEasingFunctionLibrary::EaseInBack(const double Alpha)
{
	constexpr double C1 = 1.70158;
	constexpr double C3 = C1 + 1.0;

	return C3 * FMath::Cube(Alpha) - C1 * FMath::Square(Alpha);
}

double DUEEasingFunctionLibrary::EaseOutBack(const double Alpha)
{
	constexpr double C1 = 1.70158;
	constexpr double C3 = C1 + 1;

	return 1 + C3 * FMath::Cube(Alpha - 1.0) + C1 * FMath::Square(1.0 - Alpha);
}

double DUEEasingFunctionLibrary::EaseInOutBack(const double Alpha)
{
	constexpr double C1 = 1.70158;
	constexpr double C2 = C1 * 1.525;

	if (Alpha < 0.5)
	{
		return (FMath::Cube(2.0 * Alpha) * ((C2 + 1.0) * 2.0 * Alpha - C2)) / 2.0;
	}
	return (FMath::Cube(2.0 * Alpha - 2.0) * ((C2 + 1.0) * (Alpha * 2.0 - 2.0) + C2) + 2.0) / 2.0;
}

double DUEEasingFunctionLibrary::EaseInBounce(const double Alpha)
{
	return 1.0 - EaseOutBounce(1.0 - Alpha);
}

double DUEEasingFunctionLibrary::EaseOutBounce(const double Alpha)
{
	constexpr double N1 = 7.5625;
	constexpr double D1 = 2.75;

	if (Alpha < 1 / D1)
	{
		return N1 * FMath::Square(Alpha);
	}
	if (Alpha < 2 / D1)
	{
		return N1 * ((Alpha - 1.5) / D1) * (Alpha - 1.5) + 0.75;
	}
	if (Alpha < 2.5 / D1)
	{
		return N1 * ((Alpha - 2.25) / D1) * (Alpha - 2.25) + 0.9375;
	}
	return N1 * ((Alpha - 2.625) / D1) * (Alpha - 2.625) + 0.984375;
}

double DUEEasingFunctionLibrary::EaseInOutBounce(const double Alpha)
{
	return Alpha > 0.5
		       ? (1.0 - EaseOutBounce(1.0 - 2.0 * Alpha)) / 2.0
		       : (1.0 + EaseOutBounce(2.0 * Alpha - 1.0)) / 2.0;
}
