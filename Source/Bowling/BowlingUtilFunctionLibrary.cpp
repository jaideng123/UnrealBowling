// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingUtilFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

void UBowlingUtilFunctionLibrary::SetViewTargetWithBlendForAllPlayerControllers(AActor* NewViewTarget, float BlendTime,
                                                                                EViewTargetBlendFunction BlendFunc,
                                                                                float BlendExp, bool bLockOutgoing)
{
	for(int i = 0; i < UGameplayStatics::GetNumPlayerControllers(NewViewTarget); ++i)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(NewViewTarget, i);
		if(PlayerController)
		{
			// TODO: Check Authority
			PlayerController->SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
		}
	}
}
