// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MKKS_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_PROJECT_API UMKKS_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable)
		void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

private:
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		class ATeam_ProjectCharacter* Team_ProjectCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bCombatState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bSitting;//앉아있느냐

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bCrouching;//앉아있느냐

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bLayingDown;//누워있느냐?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float MovementOffsetPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float AimOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float AimOffsetPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float LastMovementOffsetYaw;//스탑무빙 바로 전의 프레임의 OffsetYaw 값

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float MoveForwardBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float MoveRightLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bBowEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bTwoHandedEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bOneHandedEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftKnuckleEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bRightKnuckleEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftSwordEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bRightSwordEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bHammerEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bTwoHandedSwordEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftWeaponEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bRightWeaponEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bWeaponIsLeftHand;//1 손 ,0 등

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bWeaponIsRightHand;//1 손 ,0 등

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftItemEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item,
		meta = (AllowPrivateAccess = "true"))
		bool bRightItemEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bDoAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bTwohandedKick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bIsDead;

public:
	FORCEINLINE float GetMoveForwardBack() const { return MoveForwardBack; }
	FORCEINLINE float GetMoveRightLeft() const { return MoveRightLeft; }
};
