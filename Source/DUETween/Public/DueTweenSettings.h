// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DueTweenSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Plugin, defaultconfig, meta = (DisplayName="Due Tween Settings"))
class DUETWEEN_API UDueTweenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// Initial Size of Tween Pool On Load
	UPROPERTY(Config, EditAnywhere, Category=General)
	int InitialTweenPoolSize = 100;
	// Max number of Tweens that can be active at any given time
	UPROPERTY(Config, EditAnywhere, Category=General)
	int MaxTweenPoolSize = 10000;
};