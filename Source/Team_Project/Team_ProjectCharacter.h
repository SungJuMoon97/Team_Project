// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Team_ProjectCharacter.generated.h"

UCLASS(config=Game)
class ATeam_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ATeam_ProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	FTimerHandle TimerHandle;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	//Sprinting, Stamina
	void SprintStart();
	void SprintEnd();

	float RunSpeed = 600.f;
	float SprintSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool isSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float currentStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float maxStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaSprintUsageRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaRechargeRate;

	//Food, Water
	UFUNCTION(BlueprintCallable)
		void DecreaseFoodWater();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Food;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxWater;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Water;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float FoodWaterDrainRate;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

