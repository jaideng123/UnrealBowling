#pragma once
#include "ActiveDueTween.h"
#include "DUETween.h"


/**
 * Internal utilities for tweening
 */
class FDueTweenInternalUtils
{
public:
	static FValueContainer GetCurrentValueFromProperty(const FDUETweenData& TweenData);
	static void SetProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue);
	static void SetCurrentValue(const FDUETweenData& TweenData, const FValueContainer& NewValue);
};
