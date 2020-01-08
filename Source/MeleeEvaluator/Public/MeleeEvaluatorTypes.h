#pragma once

#include "Kismet/KismetSystemLibrary.h"
#include "MeleeEvaluatorTypes.generated.h"

UENUM(BlueprintType)
enum class EMeleeSweepType : uint8
{
	MTT_Box				UMETA(DisplayName = "Box"),
	MTT_Sphere			UMETA(DisplayName = "Sphere"),
	MTT_Line			UMETA(DisplayName = "Line"),
};

USTRUCT(BlueprintType)
struct FMeleeEvaluatorAttack
{
	GENERATED_BODY()

	FMeleeEvaluatorAttack()
		: SweepStartLengthFromSocket(0.f)
		, SweepEndLengthFromSocket(100.f)
		, SweepAxis(EAxis::X)
		, bPositiveSweepAxis(false)
		, SweepChannel(ECollisionChannel::ECC_Visibility)
		, SweepType(EMeleeSweepType::MTT_Line)
		, BoxSweepHeight(10.f)
		, BoxSweepWidth(10.f)
		, SphereSweepRadius(8.f)
	{}

	/** How far from the socket to start the sweep */
	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinShownByDefault))
	float SweepStartLengthFromSocket;

	/** How far from the socket to end the sweep */
	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinShownByDefault))
	float SweepEndLengthFromSocket;

	/** Aim axis for the socket */
	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	TEnumAsByte<EAxis::Type> SweepAxis;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	bool bPositiveSweepAxis;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	TEnumAsByte<ECollisionChannel> SweepChannel;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	EMeleeSweepType SweepType;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	float BoxSweepHeight;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	float BoxSweepWidth;

	UPROPERTY(EditAnywhere, Category = Melee, meta = (PinHiddenByDefault))
	float SphereSweepRadius;

	FORCEINLINE void Sweep()
	{
		//const float SweepDirection = (bPositiveSweepAxis) ? 1.f : -1.f;
		//const FVector SweepStart = SocketTM.GetLocation() + SocketTM.GetScaledAxis(SweepAxis) * SweepStartLengthFromSocket * SweepDirection;
		//const FVector SweepEnd = SocketTM.GetLocation() + SocketTM.GetScaledAxis(SweepAxis) * SweepEndLengthFromSocket * SweepDirection;

		//switch (SweepType)
		//{
		//case EMeleeSweepType::MTT_Box:
		//	BoxTrace_AnyThread(SweepStart, SweepEnd, SocketTM.Rotator(), SkelComp, OutHits);
		//	break;
		//case EMeleeSweepType::MTT_Sphere:
		//	SphereTrace_AnyThread(SweepStart, SweepEnd, SkelComp, OutHits);
		//	break;
		//case EMeleeSweepType::MTT_Line:
		//	LineTrace_AnyThread(SweepStart, SweepEnd, SkelComp, OutHits);
		//	break;
		//default:
		//	break;
		//}
	}

	FORCEINLINE void BoxTrace_AnyThread(const FVector& Start, const FVector& End, const FRotator& Orientation, USkeletalMeshComponent* SkelComp, TArray<FHitResult>& OutHits)
	{
		const FVector HalfSize{ BoxSweepHeight / 2.f, BoxSweepWidth / 2.f, 1.f };
		UKismetSystemLibrary::BoxTraceMulti(SkelComp, Start, End, HalfSize, Orientation, UEngineTypes::ConvertToTraceType(SweepChannel), false, TArray<AActor*>() = { SkelComp->GetOwner() }, EDrawDebugTrace::ForDuration, OutHits, false);
	}

	FORCEINLINE void SphereTrace_AnyThread(const FVector& Start, const FVector& End, USkeletalMeshComponent* SkelComp, TArray<FHitResult>& OutHits)
	{
		UKismetSystemLibrary::SphereTraceMulti(SkelComp, Start, End, SphereSweepRadius, UEngineTypes::ConvertToTraceType(SweepChannel), false, TArray<AActor*>() = { SkelComp->GetOwner() }, EDrawDebugTrace::ForDuration, OutHits, false);
	}

	FORCEINLINE void LineTrace_AnyThread(const FVector& Start, const FVector& End, USkeletalMeshComponent* SkelComp, TArray<FHitResult>& OutHits)
	{
		UKismetSystemLibrary::LineTraceMulti(SkelComp, Start, End, UEngineTypes::ConvertToTraceType(SweepChannel), false, TArray<AActor*>() = { SkelComp->GetOwner() }, EDrawDebugTrace::ForDuration, OutHits, false);
	}
};