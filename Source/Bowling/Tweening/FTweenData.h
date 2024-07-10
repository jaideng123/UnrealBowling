#pragma once
#include "Kismet/KismetMathLibrary.h"
#include "FTweenData.generated.h"

USTRUCT()
struct FTweenData {
	GENERATED_BODY()
public:
	UPROPERTY()
	UObject* Target;
	UPROPERTY()
	float Duration;
	UPROPERTY()
	TEnumAsByte<EEasingFunc::Type> EasingType;
	UPROPERTY()
	float BlendExp;
	UPROPERTY()
	int32 Steps;
};
