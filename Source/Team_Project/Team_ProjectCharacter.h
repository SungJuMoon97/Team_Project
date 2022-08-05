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
	void LeftHandEnd();
	void RightHandEnd();

	void InputTimeCheck();
	void CameraOption();
	void BarWidget();

	void BlockModeAim();
	void BowAiming(float DeltaTIme);
	
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

	bool bThirdPersonView;
	float BareHandDamage;//�Ǽհ��ݷ�
	float inputTime;
	float PCInputTime_Stand;
	float PCInputTime_Interact;
	float DefaultSpeed;
	float CrouchSpeed;
	float CombatSpeed;
	float SprintSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryPanel> PlayerInventoryPanelClass;

	// The Widget instance that we are using as our HUD.
	UPROPERTY()
		class UInventoryPanel* PlayerInventoryPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float Health;

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
	void GrabbedRightActorLTCheck();
	void GrabbedLeftActorRTCheck();
	void GrabbedTLCheck();
	void GrabbedTRCheck();
	bool TwoHandedCombatCheck();
	bool TwoHandedCombatGrab();
	void AttachWeaponBack();
	void DetachWeaponBack();

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

	UPROPERTY(VisibleAnywhere, Category = Character)
		class AMKKS_PlayerController* MKKS_Controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		class UMKKS_PlayerAnimInstance* PlayerAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* LeftPunchingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RightPunchingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* LeftSwordAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RightSwordAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* LeftTwoHandedSwordAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RightTwoHandedSwordAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* LeftHammerAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* RightHammerAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* BowAttackMontage;

	UPROPERTY(VisibleAnywhere)
		class UBarWidget* HealthWidget;

	UPROPERTY(VisibleAnyWhere,BlueprintReadOnly,Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class AItem* TraceItem;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class AWeapon* TraceWeapon;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
		class AItem* LeftEquippedItem;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
		class AItem* RightEquippedItem;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class AWeapon* LeftEquippedWeapon;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class AWeapon* RightEquippedWeapon;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class AWeapon* AttachedWeapon;

	//UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	//	const USkeletalMeshSocket* LeftHandSocket;

	//UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	//	const USkeletalMeshSocket* RightHandSocket;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		AItem* TraceHitItem;*/

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
		bool bLeftItemEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bRightItemEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftWeaponEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bRightWeaponEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bWeaponIsLeftHand;//1 �� ,0 ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bWeaponIsRightHand;//1 �� ,0 ��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftHandAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Motion,
		meta = (AllowPrivateAccess = "true"))
		bool bRightHandAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bBowEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bTwoHandedEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bOneHandedEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftKnuckleEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bRightKnuckleEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bLeftSwordEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bRightSwordEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bHammerEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon,
		meta = (AllowPrivateAccess = "true"))
		bool bTwoHandedSwordEquip;

private:
	/*Default camera field of view value*/
	float CameraDefaultFOV;

	/*Field of view value for when zoomed in*/
	float CameraZoomedFOV;

	/*Current field of view this frame*/
	float CameraCurrentFOV;

	float ZoomInterpSpeed;

protected:
	//Sprinting, Stamina
	void SprintStart();
	void SprintEnd();
	void Stamina(float DeltaTime);

	//CurrentHungry, Thirsty
	UFUNCTION(BlueprintCallable)
		void DecreaseFoodWater();

	// HP
	float MaxHealth;
	float CurrentHealth;
	float MaxHeadHealth;
	float MaxBodyHealth;
	float MaxRightArmHealth;
	float MaxLeftArmHealth;
	float MaxRightLegHealth;
	float MaxLeftLegHealth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float currentStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float MaxStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaSprintUsageRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaRechargeRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float MaxHungry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float CurrentHungry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float MaxThirsty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float CurrentThirsty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float FoodWaterDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float HungryRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
		float ThirstyRate;
public:
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
	FORCEINLINE bool GetIsSprint() const { return bSprint; }
	FORCEINLINE bool GetLeftWeaponEquip() const { return bLeftWeaponEquip; }
	FORCEINLINE bool GetLeftItemEquip() const { return bLeftItemEquip; }
	FORCEINLINE bool GetRightWeaponEquip() const { return bRightWeaponEquip; }
	FORCEINLINE bool GetRightItemEquip() const { return bRightItemEquip; }
	FORCEINLINE bool GetLeftSwordEquip() const { return bLeftSwordEquip; }
	FORCEINLINE bool GetLeftKnuckleEquip() const { return bLeftKnuckleEquip; }
	FORCEINLINE bool GetRightSwordEquip() const { return bRightSwordEquip; }
	FORCEINLINE bool GetRightKnuckleEquip() const { return bRightKnuckleEquip; }
	FORCEINLINE bool GetTwoHandedSwordEquip() const { return bTwoHandedSwordEquip; }
	FORCEINLINE bool GetHammerEquip() const { return bHammerEquip; }
	FORCEINLINE bool GetBowEquip() const { return bBowEquip; }
	FORCEINLINE bool GetDoAttacking() const { return bDoAttacking; }
	FORCEINLINE bool GetTwoHandedEquip() const { return bTwoHandedEquip; }
};

