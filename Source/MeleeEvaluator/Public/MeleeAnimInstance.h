// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MeleeAnimInstance.generated.h"

class UMeleeEvaluatorComponent;

/**
 * 
 */
UCLASS()
class MELEEEVALUATOR_API UMeleeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	virtual void NativeInitializeAnimation() override;
	virtual void NativePostEvaluateAnimation() override;

protected:
	TWeakObjectPtr<UMeleeEvaluatorComponent> MeleeComp;
};
