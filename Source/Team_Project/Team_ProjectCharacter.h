// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Team_ProjectCharacter.generated.h"

UCLASS(config=Game)
class ATeam_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATeam_ProjectCharacter();

	virtual void BeginPlay() override;

	void DoAttack();
	void EndAttack();


public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	// Widgetclass to spawn for the heads up display.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UBarWidget> PlayerWidgetClass;

	// The Widget instance that we are using as our HUD.
	UPROPERTY()
	class UBarWidget* PlayerWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryPanel> PlayerInventoryPanelClass;

	// The Widget instance that we are using as our HUD.
	UPROPERTY()
	class UInventoryPanel* PlayerInventoryPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

protected:

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
	// End of APawn interface

	// Pickup and Drop Key
	void Interact();
	void GrabActor();
	void ReleaseActor();
	AActor* HeldActor;

	// Add Item To Inventory Key
	void AddToInventory();
	void PutActor();

	// Open Inventory
	void OpenInventory();
	bool bIsInventoryOpen;

	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class AItem* Item);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterInventory")
	class UInventoryComponent* Inventory;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

private:

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetInventoryOpen() const { return bIsInventoryOpen; }
};

