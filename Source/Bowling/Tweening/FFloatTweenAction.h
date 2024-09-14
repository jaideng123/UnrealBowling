#pragma once
#include "DUETween.h"
#include "FTweenData.h"
#include "LatentActions.h"
#include "TweenFunctions.h"
#include "FFloatTweenAction.generated.h"


USTRUCT()
struct FFloatTweenData : public FTweenData {
	GENERATED_BODY()

public:
	UPROPERTY()
	float TargetValue;
	UPROPERTY()
	FName FieldName;
};


// FTweenAction
// Tweens from one location to another over a duration
class FFloatTweenAction : public FPendingLatentAction {
public:
	FName           ExecutionFunction;
	int32           OutputLink;
	FWeakObjectPtr  CallbackTarget;
	FFloatTweenData TweenData;

private:
	float                 TimeElapsed;
	float                 StartingValue;
	const FFloatProperty* FloatProperty;
	FActiveDueTween* ActiveDueTween;

public:
	FFloatTweenAction(const FLatentActionInfo& LatentInfo, FFloatTweenData TweenData)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
		  , TweenData(TweenData)
		  , TimeElapsed(0)
	{
		FProperty* propertyRef = TweenData.Target->GetClass()->FindPropertyByName(TweenData.FieldName);
		FDUETweenData DUETweenData;
		DUETweenData.Duration = TweenData.Duration;
		DUETweenData.Steps = TweenData.Steps;
		DUETweenData.Target = TweenData.Target;
		DUETweenData.BlendExp = TweenData.BlendExp;
		// TODO: Correct this
		DUETweenData.EasingType = EasingType::Linear;
		DUETweenData.EndingValue = FValueContainer(TweenData.TargetValue);
		DUETweenData.TargetProperty = propertyRef;
		DUETweenData.ValueType = EValueType::Float;
		

		// TODO: move this into due tween
		if(propertyRef)
		{
			FloatProperty = CastField<FFloatProperty>(propertyRef);
			if(FloatProperty && TweenData.Target.IsValid())
			{
				FloatProperty->GetValue_InContainer(TweenData.Target.Get(), &StartingValue);
				
				DUETweenData.StartingValue = FValueContainer(StartingValue);
				ActiveDueTween = FDUETweenModule::Get().AddTween(DUETweenData);
				return;
			}
		}

		UE_LOG(LogTemp, Error, TEXT("No Float Property Found For:%s"), *TweenData.FieldName.ToString());
	}

	virtual void UpdateOperation(FLatentResponse& Response) override;

#if WITH_EDITOR
	// Returns a human readable description of the latent operation's current state
	virtual FString GetDescription() const override
	{
		static const FNumberFormattingOptions DelayTimeFormatOptions = FNumberFormattingOptions()
		                                                               .SetMinimumFractionalDigits(3)
		                                                               .SetMaximumFractionalDigits(3);
		return FText::Format(
			NSLOCTEXT("FloatTweenAction", "DelayActionTimeFmt", "Tween ({0} seconds left)"),
			FText::AsNumber(TweenData.Duration - TimeElapsed, &DelayTimeFormatOptions)).ToString();
	}
#endif
};
