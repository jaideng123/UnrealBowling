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
	static void SetCurrentValueToProperty(const FDUETweenData& TweenData, const FValueContainer& NewValue);
};