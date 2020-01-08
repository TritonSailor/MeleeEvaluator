// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAnimInstance.h"
#include "MeleeAnimInstanceProxy.h"
#include "MeleeEvaluatorInterface.h"
#include "MeleeEvaluatorComponent.h"

FAnimInstanceProxy* UMeleeAnimInstance::CreateAnimInstanceProxy()
{
	return new FMeleeAnimInstanceProxy(this);
}

void UMeleeAnimInstance::NativeInitializeAnimation()
{
	//MeleeComp = (TryGetPawnOwner() && TryGetPawnOwner()->Implements<UMeleeEvaluatorInterface>()) ? IMeleeEvaluatorInterface::Execute_GetMeleeEvaluatorComponent(TryGetPawnOwner()) : nullptr;
}

void UMeleeAnimInstance::NativePostEvaluateAnimation()
{
	if (MeleeComp.IsValid())
	{
		FMeleeAnimInstanceProxy& Proxy = GetProxyOnGameThread<FMeleeAnimInstanceProxy>();
		if (Proxy.SweepTransforms.Num() > 0)
		{
			MeleeComp->OnProcessSweepTransforms(Proxy.SweepTransforms);
			Proxy.SweepTransforms.Reset();
		}
	}
	else if (TryGetPawnOwner() && TryGetPawnOwner()->Implements<UMeleeEvaluatorInterface>())
	{
		MeleeComp = IMeleeEvaluatorInterface::Execute_GetMeleeEvaluatorComponent(TryGetPawnOwner());
	}
}
