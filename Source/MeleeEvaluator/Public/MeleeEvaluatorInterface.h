#pragma once

#include "MeleeEvaluatorInterface.generated.h"

UINTERFACE(BlueprintType)
class MELEEEVALUATOR_API UMeleeEvaluatorInterface : public UInterface
{
	GENERATED_BODY()
};

class MELEEEVALUATOR_API IMeleeEvaluatorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = MeleeEvaluator)
	void OnHit(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, Category = MeleeEvaluator)
	UMeleeEvaluatorComponent* GetMeleeEvaluatorComponent() const;
};