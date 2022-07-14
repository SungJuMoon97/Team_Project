// Fill out your copyright notice in the Description page of Project Settings.


#include "DealDamageNotify.h"
#include "Team_ProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

void UDealDamageNotify::HandCheck(ATeam_ProjectCharacter* Character)
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Team_ProjectCharacter);
	bool bResult;
	FVector StartLocation;
	FVector EndLocation;
	
	Character = Team_ProjectCharacter;

	if (Character->GetLeftHandAction())
	{
		StartLocation = Character->GetMesh()->GetSocketLocation(TEXT("hand_l"));
		EndLocation = Character->GetMesh()->GetSocketLocation(TEXT("hand_l"));
	}
	else
	{
		StartLocation = Character->GetMesh()->GetSocketLocation(TEXT("hand_r"));
		EndLocation = Character->GetMesh()->GetSocketLocation(TEXT("hand_r"));
	}

	if (Character->GetLeftHandAction())
	{
		bResult = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_EngineTraceChannel2,
			FCollisionShape::MakeSphere(5.0f),
			Params);

		if (bResult)
		{
			FColor DrawColor = bResult ? FColor::Green : FColor::Red;
			const float DebugLifeTime = 1.0f;
			DrawDebugSphere(GetWorld(), StartLocation,5.0f,2.5f, DrawColor,true, DebugLifeTime);
		}
	}
}
