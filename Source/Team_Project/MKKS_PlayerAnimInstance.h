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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		class ATeam_ProjectCharacter* Team_ProjectCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bCombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bSitting;//앉아있느냐

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bLyingDown;//누워있느냐?

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		float LastMovementOffsetYaw;//스탑무빙 바로 전의 프레임의 OffsetYaw 값

};
