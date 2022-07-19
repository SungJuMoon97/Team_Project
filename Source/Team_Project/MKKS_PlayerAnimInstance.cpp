// Fill out your copyright notice in the Description page of Project Settings.


#include "MKKS_PlayerAnimInstance.h"
#include "Enum_Collection.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Team_ProjectCharacter.h"
#include "MKKS_PlayerController.h"

void UMKKS_PlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Team_ProjectCharacter == nullptr)
	{
		Team_ProjectCharacter = Cast<ATeam_ProjectCharacter>(TryGetPawnOwner());
	}

	if (Team_ProjectCharacter)
	{
		FVector Velocity{ Team_ProjectCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = Team_ProjectCharacter->GetCharacterMovement()->IsFalling();

		if (Team_ProjectCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		FRotator AimRotation = Team_ProjectCharacter->GetBaseAimRotation();

		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(
			Team_ProjectCharacter->GetVelocity());

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation, AimRotation).Yaw;

		MoveForwardBack = Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Forward / Backward"));
		MoveRightLeft = Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Right / Left"));

		if (Team_ProjectCharacter->GetVelocity().Size() > 0.0f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
		bSprint = Team_ProjectCharacter->GetIsSprint();

		bCombatState = Team_ProjectCharacter->GetCombatState();

		bSitting = Team_ProjectCharacter->GetSitting();

		bCrouching = Team_ProjectCharacter->GetCrouching();

		bLayingDown = Team_ProjectCharacter->GetLyingDown();

		if (bSitting || bCrouching || bLayingDown)
		{
			bSprint = false;
		
			if (bSitting || bCrouching && bLayingDown)
			{
				bSitting = false;
				bCrouching = false;
			}
		}

		if ((Team_ProjectCharacter->CurrentStanceMode == EStance::ES_Combat)&&(bLayingDown == true))
		{
			bCombatState = false;
			Team_ProjectCharacter->CurrentStanceMode = EStance::ES_Default;
			Team_ProjectCharacter->SetStanceType(Team_ProjectCharacter->CurrentStanceMode);
			
		}

		if ((Team_ProjectCharacter->CurrentStanding == EStanding::ESD_LayingDown) && (MoveForwardBack != 0.0f || MoveRightLeft != 0.0f))
		{
			bCrouching = true;
			bLayingDown = false;
			Team_ProjectCharacter->CurrentStanding = EStanding::ESD_Crouching;
			Team_ProjectCharacter->SetStanding(Team_ProjectCharacter->CurrentStanding);
			UE_LOG(LogTemp, Warning, TEXT("Laying to Crouch"));
		}

	}

}

void UMKKS_PlayerAnimInstance::NativeInitializeAnimation()
{
	Team_ProjectCharacter = Cast<ATeam_ProjectCharacter>(TryGetPawnOwner());
}


