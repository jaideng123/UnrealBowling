// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPlayerState.h"

void ABowlingPlayerState::ReportPins(int numPins)
{
	if(Frames.Num() < (CurrentFrame + 1))
	{
		Frames.Add(FBowlingFrame());
	}

	if(CurrentBall == 0)
	{
		Frames[CurrentFrame].ball1Pins = numPins;
		if(numPins == 10)
		{
			Frames[CurrentFrame].wasStrike = true;
		}
	}
	if(CurrentBall == 1)
	{
		Frames[CurrentFrame].ball2Pins = numPins;
		if(numPins + Frames[CurrentFrame].ball1Pins == 10)
		{
			Frames[CurrentFrame].wasSpare = true;
		}
	}

	CurrentBall++;
	
	if(CurrentBall == 2 || Frames[CurrentFrame].wasStrike)
	{
		CurrentBall = 0;
		CurrentFrame++;
	}
	
}

void ABowlingPlayerState::TestPins()
{
	ReportPins(FMath::RandRange(0,10));
}
