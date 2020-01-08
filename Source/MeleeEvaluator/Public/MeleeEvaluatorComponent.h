#pragma once

#include "Components/ActorComponent.h"
#include "MeleeEvaluatorComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class UMeleeEvaluatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = MeleeEvaluator)
	void StartMelee(UAnimSequenceBase* NewAnim, float AnimBlendOutTime = 0.2f)
	{
		bRestartPending = IsAttacking();
		if (bRestartPending) { bStarted = false; }

		Sequence = NewAnim;
		StartTime = GetWorld()->GetTimeSeconds();
		EndTime = GetWorld()->GetTimeSeconds() + GetSequenceLength();
		BlendOutTime = AnimBlendOutTime;
	}

	UFUNCTION(BlueprintCallable, Category = MeleeEvaluator)
	void StopMelee()
	{
		Sequence = nullptr; 
		bStarted = false;
		StartTime = -1.f;
		EndTime = -1.f;
	}

	/** 
	 * Do your sweeps here using the SweepOrigins array 
	*/
	UFUNCTION(BlueprintNativeEvent, Category = MeleeEvaluator)
	void OnProcessSweepTransforms(const TArray<FTransform>& SweepTransforms);
	virtual void OnProcessSweepTransforms_Implementation(const TArray<FTransform>& SweepTransforms)
	{
		// Process sweeps here
	}

	UFUNCTION(BlueprintPure, Category = MeleeEvaluator)
	bool IsAttacking() const
	{
		return Sequence != nullptr && bStarted && GetWorld()->GetTimeSeconds() >= StartTime && GetWorld()->GetTimeSeconds() <= EndTime - BlendOutTime;
	}

	bool IsEvaluating() const
	{
		return bStarted && GetWorld()->GetTimeSeconds() <= EndTime;
	}

	FORCEINLINE UAnimSequenceBase* GetSequence() const { return Sequence; }

	FORCEINLINE float GetSequenceLength() const
	{
		if (Sequence)
		{
			return Sequence->GetPlayLength();
		}
		return -1.f;
	}

protected:
	UAnimSequenceBase* Sequence;

public:
	float StartTime;
	float EndTime;
	float BlendOutTime;
	bool bStarted;
	bool bRestartPending;

public:
	UMeleeEvaluatorComponent()
		: Sequence(nullptr)
		, StartTime(-1.f)
		, EndTime(-1.f)
		, BlendOutTime(0.f)
		, bStarted(false)
	{}
};
