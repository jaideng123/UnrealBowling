// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchTimer.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UTouchTimer::UpdatePosition(FVector2D Position)
{
	SetPositionInViewport(Position);
}
