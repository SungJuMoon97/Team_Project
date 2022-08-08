// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enum_Collection.h"
#include "Team_ProjectCharacter.generated.h"

UCLASS(config=Game)
class ATeam_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATeam_ProjectCharacter();

	void LeftHand();
	void RightHand();

	void InputTimeCheck();
	void CameraOption();
	void BarWidget();

	UFUNCTION()
		void SetViewType(EViewType ViewType);
	
	UFUNCTION()
		void ViewChange();

	UFUNCTION()
		void StanceChange();

	UFUNCTION()
		void SetStanceType(EStance StanceType);

	UFUNCTION()
		void SetStanding(EStanding StandingType);

	UFUNCTION()
		void StandingChange();

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

	FTimerHandle TimerHandle;
	EViewType CurrentViewMode;
	EStance CurrentStanceMode;
	EStanding CurrentStanding;
	EWeaponType CurrentWeapon;
	EWeaponHand CurrentHandWeapon;
	bool bThirdPersonView;
	float BareHandDamage;//�Ǽհ��ݷ�
	float inputTime;
	float PCInputTime;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryPanel> PlayerInventoryPanelClass;

	// The Widget instance that we are using as our HUD.
	UPROPERTY()
	class UInventoryPanel* PlayerInventoryPanel;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHeadHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxBodyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxRightArmHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxLeftArmHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxRightLegHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxLeftLegHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHeadHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentBodyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentRightArmHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentLeftArmHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentRightLegHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentLeftLegHealth;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	bool bIsHoldingItem;

	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class AItem* Item);
private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* FirstPersonCameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterInventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* ThirdPersonCameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ThirdPersonFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class AMKKS_PlayerController* MKKS_Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UMKKS_PlayerAnimInstance* PlayerAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* LeftPunchingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RightPunchingMontage;

	UPROPERTY(VisibleAnywhere)
	class UBarWidget* HealthWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* HeadDamageBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BodyDamageBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightArmDamageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightArmDamageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftArmDamageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftArmDamageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightLegDamageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightLegDamageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RightLegDamageBox3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftLegDamageBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftLegDamageBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageBox, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LeftLegDamageBox3;



private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bDoAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bCrouching;//��ũ�����ִ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bSitting;//�ɾ��ִ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bLayingDown;//�����ִ���?

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bCombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bLeftHandAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
	bool bRightHandAction;

protected:
	//Sprinting, Stamina
	void SprintStart();
	void SprintEnd();
	void Stamina(float DeltaTime);

	float RunSpeed = 600.f;
	float SprintSpeed = 1200.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bIsSprinting;

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
	FORCEINLINE class USpringArmComponent* GetFirstPersonCameraBoom() const { return FirstPersonCameraBoom; }
	FORCEINLINE class UCameraComponent* GetFirstPersonFollowCamera() const { return FirstPersonFollowCamera; }
	FORCEINLINE class USpringArmComponent* GetThirdPersonCameraBoom() const { return ThirdPersonCameraBoom; }
	FORCEINLINE class UCameraComponent* GetThirdPersonFollowCamera() const { return ThirdPersonFollowCamera; }

	FORCEINLINE bool GetSitting() const { return bSitting; }

	FORCEINLINE bool GetCrouching() const { return bCrouching; }

	FORCEINLINE bool GetLyingDown() const { return bLayingDown; }

	FORCEINLINE bool GetCombatState() const { return bCombatState; }

	FORCEINLINE bool GetLeftHandAction() const { return bLeftHandAction; }

	FORCEINLINE bool GetRightHandAction() const { return bRightHandAction; }

	
	FORCEINLINE bool GetInventoryOpen() const { return bIsInventoryOpen; }
};

