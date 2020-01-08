#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "MeleeAnimInstanceProxy.generated.h"

USTRUCT()
struct FMeleeAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	
	typedef FAnimInstanceProxy Super;

public:
	TArray<FTransform> SweepTransforms;

	FMeleeAnimInstanceProxy()
		: Super()
	{}

	FMeleeAnimInstanceProxy(UAnimInstance* Instance)
		: Super(Instance)
	{}
};