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

		MovementOffsetPitch = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation, AimRotation).Pitch;

		AimOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(Team_ProjectCharacter->GetBaseAimRotation(),Team_ProjectCharacter->GetActorRotation()).Yaw;
		AimOffsetPitch = UKismetMathLibrary::NormalizedDeltaRotator(Team_ProjectCharacter->GetBaseAimRotation(), Team_ProjectCharacter->GetActorRotation()).Pitch;
		
		MoveForwardBack = //UKismetMathLibrary::NormalizeAxis(Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Forward / Backward")));
			Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Forward / Backward"));
		MoveRightLeft = //UKismetMathLibrary::NormalizeAxis(Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Right / Left")));
			Team_ProjectCharacter->GetInputAxisValue(TEXT("Move Right / Left"));
		

		if (Team_ProjectCharacter->GetVelocity().Size() > 0.0f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
		bDoAttacking = Team_ProjectCharacter->GetDoAttacking();

		bSprint = Team_ProjectCharacter->GetIsSprint();

		bCombatState = Team_ProjectCharacter->GetCombatState();

		bSitting = Team_ProjectCharacter->GetSitting();

		bCrouching = Team_ProjectCharacter->GetCrouching();

		bLayingDown = Team_ProjectCharacter->GetLyingDown();

		bLeftSwordEquip = Team_ProjectCharacter->GetLeftSwordEquip();

		bLeftKnuckleEquip = Team_ProjectCharacter->GetLeftKnuckleEquip();

		bRightSwordEquip = Team_ProjectCharacter->GetRightSwordEquip();

		bRightKnuckleEquip = Team_ProjectCharacter->GetRightKnuckleEquip();

		bTwoHandedSwordEquip = Team_ProjectCharacter->GetTwoHandedSwordEquip();

		bHammerEquip = Team_ProjectCharacter->GetHammerEquip();

		bBowEquip = Team_ProjectCharacter->GetBowEquip();

		bLeftWeaponEquip = Team_ProjectCharacter->GetLeftWeaponEquip();

		bLeftItemEquip = Team_ProjectCharacter->GetLeftItemEquip();

		bRightWeaponEquip = Team_ProjectCharacter->GetRightWeaponEquip();

		bRightItemEquip = Team_ProjectCharacter->GetRightItemEquip();

		bTwoHandedEquip = Team_ProjectCharacter->GetTwoHandedEquip();

		bIsDead = Team_ProjectCharacter->GetIsDead();

		bTwohandedKick = Team_ProjectCharacter->GetTwoHandedKick();

		if (bSprint && MoveRightLeft != 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("We have stopped sprinting."));
			Team_ProjectCharacter->CurrentStanding = EStanding::ESD_Standing;
			Team_ProjectCharacter->SetStanding(Team_ProjectCharacter->CurrentStanding);
		}

		if (bSitting || bCrouching || bLayingDown || bIsDead)
		{
			bSprint = false;
		
			if (bSitting || bCrouching && (bLayingDown || bIsDead))
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


