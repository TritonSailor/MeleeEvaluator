#include "MeleeEvaluatorNode.h"
#include "MeleeAnimInstanceProxy.h"
#include "MeleeAnimInstance.h"
#include "MeleeEvaluatorInterface.h"
#include "MeleeEvaluatorComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeEvaluatorNodeNode, Log, All);


void FAnimNode_MeleeEvaluatorNode::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	FAnimNode_SequenceEvaluator::Initialize_AnyThread(Context);

	bValidProxy = Context.AnimInstanceProxy->GetAnimInstanceObject()->IsA(UMeleeAnimInstance::StaticClass());
	if (!bValidProxy)
	{
		UE_LOG(LogMeleeEvaluatorNodeNode, Error, TEXT("Animation Instance must inherit from UMeleeAnimInstance. Unable to evaluate."));
	}

	RemainingTime = 0.f;
}

void FAnimNode_MeleeEvaluatorNode::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	FAnimNode_SequenceEvaluator::CacheBones_AnyThread(Context);

	USkeletalMeshComponent* const SkelComp = Context.AnimInstanceProxy->GetSkelMeshComponent();
	if (SkelComp)
	{
		const FBoneContainer& RequiredBones = Context.AnimInstanceProxy->GetRequiredBones();

		Bone = SkelComp->GetSocketBoneName(WeaponSocket);
		Bone.Initialize(RequiredBones);
		BoneIndex = Bone.GetCompactPoseIndex(RequiredBones);
	}
}

void FAnimNode_MeleeEvaluatorNode::UpdateAssetPlayer(const FAnimationUpdateContext& Context)
{
	GetEvaluateGraphExposedInputs().Execute(Context);

	if (!bValidProxy)
	{
		return;
	}

	// This will set the explicit time, create tick record, and disable bReinitialized
	//FAnimNode_SequenceEvaluator::UpdateAssetPlayer(Context);

	ExplicitTime = InternalTimeAccumulator;

	RemainingTime += Context.GetDeltaTime();

	// Fixed step simulation at 120hz
	FixedTimeStep = (1.f / SimFrequency) * TimeDilation;
}

void FAnimNode_MeleeEvaluatorNode::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	check(Output.AnimInstanceProxy != nullptr);

	if (!bValidProxy || !MeleeComp.IsValid())
	{
		Output.ResetToRefPose();
		return;
	}

	if (MeleeComp->IsEvaluating())
	{
		TArray<FHitResult> Hits;

		USkeletalMeshComponent* const SkelComp = Output.AnimInstanceProxy->GetSkelMeshComponent();
		const FTransform& MeshTM = SkelComp->GetComponentTransform();

		const float LastITA = InternalTimeAccumulator;
		InternalTimeAccumulator += Output.AnimInstanceProxy->GetDeltaSeconds();

		RemainingTime = InternalTimeAccumulator - LastITA;
		float ITA = LastITA;
		if (!FMath::IsNearlyZero(RemainingTime, KINDA_SMALL_NUMBER) && RemainingTime >= FixedTimeStep)
		{
			while (RemainingTime > FixedTimeStep)
			{
				if (FMath::IsNearlyEqual(InternalTimeAccumulator, MeleeComp->GetSequenceLength(), KINDA_SMALL_NUMBER))
				{
					RemainingTime = 0.f;
				}

				InternalTimeAccumulator += FixedTimeStep;
				InternalTimeAccumulator = FMath::Min(InternalTimeAccumulator, MeleeComp->GetSequenceLength());

				Melee_AnyThread(Output, InternalTimeAccumulator, SkelComp, MeshTM, Hits);

				RemainingTime -= FixedTimeStep;
			}
		}
		else
		{
			Melee_AnyThread(Output, InternalTimeAccumulator, SkelComp, MeshTM, Hits);
		}
	}
	else
	{
		Output.ResetToRefPose();
	}
}

void FAnimNode_MeleeEvaluatorNode::Melee_AnyThread(FPoseContext& Output, float ITA, USkeletalMeshComponent* const SkelComp, FTransform MeshTM, TArray<FHitResult>& OutHits)
{
	if ((Sequence != nullptr) && (Output.AnimInstanceProxy->IsSkeletonCompatible(Sequence->GetSkeleton())))
	{
		Sequence->GetAnimationPose(Output.Pose, Output.Curve, FAnimExtractContext(ITA, Output.AnimInstanceProxy->ShouldExtractRootMotion()));
	}
	else
	{
		Output.ResetToRefPose();
	}

	if (!WeaponSocket.IsNone() && SkelComp && SkelComp->DoesSocketExist(WeaponSocket))
	{
		FCSPose<FCompactPose> CPose;
		CPose.InitPose(Output.Pose);

		FTransform SocketTM = CPose.GetComponentSpaceTransform(BoneIndex) * MeshTM;
		FMeleeAnimInstanceProxy* Proxy = ((FMeleeAnimInstanceProxy*)Output.AnimInstanceProxy);
		Proxy->SweepTransforms.Add(SocketTM);

	}
}

void FAnimNode_MeleeEvaluatorNode::PreUpdate(const UAnimInstance* InAnimInstance)
{
	if (!bValidProxy) 
	{
		return; 
	}

	const USkeletalMeshComponent* SkelComp = InAnimInstance->GetSkelMeshComponent();
	const UWorld* World = SkelComp->GetWorld();
	check(World->GetWorldSettings());
	TimeDilation = World->GetWorldSettings()->GetEffectiveTimeDilation();

	if (!MeleeComp.IsValid())
	{
		if (SkelComp->GetOwner() && SkelComp->GetOwner()->Implements<UMeleeEvaluatorInterface>())
		{
			MeleeComp = IMeleeEvaluatorInterface::Execute_GetMeleeEvaluatorComponent(SkelComp->GetOwner());
		}

		if (!MeleeComp.IsValid())
		{
			return;
		}
	}

	const float WorldTime = World->GetTimeSeconds();

	if (MeleeComp->bRestartPending)
	{
		InternalTimeAccumulator = 0.f;
		MeleeComp->bRestartPending = false;
	}

	// Stop melee
	if (MeleeComp->bStarted)
	{
		if (InternalTimeAccumulator >= MeleeComp->GetSequenceLength())
		{
			InternalTimeAccumulator = 0.f;
			MeleeComp->StopMelee();
		}
	}
	// Start melee
	else
	{
		if (MeleeComp->GetSequence() && WorldTime >= MeleeComp->StartTime)
		{
			MeleeComp->bStarted = true;
			InternalTimeAccumulator = 0.f;
			Sequence = MeleeComp->GetSequence();
			//OverrideAsset(MeleeComp->GetSequence());
		}
	}
}
