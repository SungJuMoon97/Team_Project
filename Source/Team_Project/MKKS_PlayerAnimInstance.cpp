// Fill out your copyright notice in the Description page of Project Settings.


#include "MKKS_PlayerAnimInstance.h"
#include "Enum_Collection.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Team_ProjectCharacter.h"

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

		if (Team_ProjectCharacter->GetVelocity().Size() > 0.0f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		if(Team_ProjectCharacter)
			bSitting = Team_ProjectCharacter->GetSitting();
		else
			bLyingDown = Team_ProjectCharacter->GetLyingDown();

	}

}

void UMKKS_PlayerAnimInstance::NativeInitializeAnimation()
{
	Team_ProjectCharacter = Cast<ATeam_ProjectCharacter>(TryGetPawnOwner());
}
