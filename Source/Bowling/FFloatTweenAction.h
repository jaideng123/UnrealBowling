#pragma once
#include "FTweenData.h"
#include "LatentActions.h"
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

public:
	FFloatTweenAction(const FLatentActionInfo& LatentInfo, FFloatTweenData TweenData)
		: ExecutionFunction(LatentInfo.ExecutionFunction)
		  , OutputLink(LatentInfo.Linkage)
		  , CallbackTarget(LatentInfo.CallbackTarget)
		  , TweenData(TweenData)
		  , TimeElapsed(0)
	{
		FProperty* propertyRef = TweenData.Target->GetClass()->FindPropertyByName(TweenData.FieldName);
		if(propertyRef)
		{
			FloatProperty = CastField<FFloatProperty>(propertyRef);
			if(FloatProperty)
			{
				FloatProperty->GetValue_InContainer(TweenData.Target, &StartingValue);
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
