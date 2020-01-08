#pragma once

#include "MeleeEvaluatorNode.h"
#include "AnimGraphNode_AssetPlayerBase.h"
#include "AnimGraphNode_MeleeEvaluator.generated.h"


UCLASS()
class MELEEEVALUATOREDITOR_API UAnimGraphNode_MeleeEvaluator : public UAnimGraphNode_AssetPlayerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_MeleeEvaluatorNode Node;

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FString GetNodeCategory() const override;
	// End of UEdGraphNode

	// UAnimGraphNode_Base interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const override;
	// End of UAnimGraphNode_Base interface
};