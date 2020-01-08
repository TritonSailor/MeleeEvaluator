#pragma once

#include "AnimNodes/AnimNode_SequenceEvaluator.h"
#include "MeleeEvaluatorNode.generated.h"

class UMeleeEvaluatorComponent;


USTRUCT(BlueprintInternalUseOnly)
struct MELEEEVALUATOR_API FAnimNode_MeleeEvaluatorNode : public FAnimNode_SequenceEvaluator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Melee)
	float SimFrequency;

	/** Socket on character mesh weapon is attached to */
	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinShownByDefault))
	FName WeaponSocket;

protected:
	/** Internal use - Amount of time we need to simulate. */
	float RemainingTime;

	/** Internal use - Current timestep */
	float FixedTimeStep;

	/** Internal use - Current time dilation */
	float TimeDilation;

	bool bValidProxy;

	FBoneReference Bone;
	FCompactPoseBoneIndex BoneIndex;

	TWeakObjectPtr<UMeleeEvaluatorComponent> MeleeComp;

public:
	FAnimNode_MeleeEvaluatorNode()
		: SimFrequency(120.f)
		, WeaponSocket("hand_r")
		, RemainingTime(0.f)
		, FixedTimeStep(0.f)
		, TimeDilation(0.f)
		, bValidProxy(false)
		, Bone(FBoneReference())
		, BoneIndex(-1)
		, MeleeComp(nullptr)
	{}

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) final;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) final;
	virtual void UpdateAssetPlayer(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) final;

	void Melee_AnyThread(FPoseContext& Output, float ITA, USkeletalMeshComponent* const SkelComp, FTransform MeshTM, TArray<FHitResult>& OutHits);

	virtual bool HasPreUpdate() const final { return true; }
	virtual void PreUpdate(const UAnimInstance* InAnimInstance) final;
	// End of FAnimNode_Base interface
};