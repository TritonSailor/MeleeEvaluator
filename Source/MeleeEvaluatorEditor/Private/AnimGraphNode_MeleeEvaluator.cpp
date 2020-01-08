#include "AnimGraphNode_MeleeEvaluator.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "A3Nodes"

FText UAnimGraphNode_MeleeEvaluator::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("MeleeEvalTitle", "Melee Evaluator");
}

FLinearColor UAnimGraphNode_MeleeEvaluator::GetNodeTitleColor() const
{
	return FLinearColor::Black;
}

FString UAnimGraphNode_MeleeEvaluator::GetNodeCategory() const
{
	return TEXT("MeleeEvaluator");
}

void UAnimGraphNode_MeleeEvaluator::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	Super::CustomizeDetails(DetailBuilder);

	TSharedRef<IPropertyHandle> NodeHandle = DetailBuilder.GetProperty(FName(TEXT("Node")), GetClass());

	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, ExplicitTime)));
	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, bShouldLoop)));
	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, Sequence)));
	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, bTeleportToExplicitTime)));
	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, StartPosition)));
	DetailBuilder.HideProperty(NodeHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAnimNode_MeleeEvaluatorNode, ReinitializationBehavior)));
}

void UAnimGraphNode_MeleeEvaluator::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
	if (Pin->PinName == GET_MEMBER_NAME_STRING_CHECKED(FAnimNode_MeleeEvaluatorNode, ExplicitTime))
	{
		Pin->bHidden = true;
	}
}

#undef LOCTEXT_NAMESPACE