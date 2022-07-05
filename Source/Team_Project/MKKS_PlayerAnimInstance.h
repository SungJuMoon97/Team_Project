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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack,
		meta = (AllowPrivateAccess = "true"))
		bool bDoAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bSitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
		meta = (AllowPrivateAccess = "true"))
		bool bLyingDown;



};
