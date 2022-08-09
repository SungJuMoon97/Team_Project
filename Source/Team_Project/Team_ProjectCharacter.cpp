
#include "Team_ProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "BarWidget.h"
#include "Blueprint/UserWidget.h"
#include "MKKS_PlayerController.h"
#include "Enum_Collection.h"
#include "MKKS_PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "MKKS_PlayerAnimInstance.h"
#include "InventoryComponent.h"
#include "InstanceItem.h"
#include "Pickup_Interface.h"
#include "Team_ProjectGameMode.h"
#include "InventoryPanel.h"
#include "Item.h"
#include "Weapon.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

ATeam_ProjectCharacter::ATeam_ProjectCharacter() :
	//if your View and Stance make a change
	CurrentViewMode(EViewType::EVT_FirstPerson), CurrentStanceMode(EStance::ES_Default), bIsDead(false),
	bCombatState(false), bSprint(false), bLeftItemEquip(false), bRightItemEquip(false), bLeftWeaponEquip(false), bRightWeaponEquip(false), bAiming(false),
	//CurrentSpeed
	DefaultSpeed(500.0f), CrouchSpeed(200.0f), CombatSpeed(200.0f), SprintSpeed(800.0f),
	//if you HandAction Default Setting
	BareHandDamage(10), bLeftHandAction(false), bRightHandAction(false), bDoAttacking(false), bBlocking(false),
	//if Character Sitting or Lying or Standing
	CurrentStanding(EStanding::ESD_Standing),
	bSitting(false), bLayingDown(false), bCrouching(false),
	inputTime(2.0f),
	bIsInventoryOpen(false),
	//PlayerStat
	Health(100.f), MaxStamina(1.0f), MaxHungry(100.0f), MaxThirsty(100.0f),
	currentStamina(1.0f), staminaSprintUsageRate(0.05f), staminaRechargeRate(0.01f),
	CurrentHungry(100.0f), CurrentThirsty(100.f), HungryRate(0.5f), ThirstyRate(0.8f),
	FoodWaterDrainRate(10.0f),//배고픔목마름줄어드는시간
	//BowAiming Setting
	CameraCurrentFOV(90.0f), CameraDefaultFOV(90.0f), CameraZoomedFOV(50.0f), ZoomInterpSpeed(20.0f),
	MaxHealth(600.f), MaxHeadHealth(100.f), MaxBodyHealth(100.f), MaxRightArmHealth(100.f), MaxLeftArmHealth(100.f), MaxRightLegHealth(100.f), MaxLeftLegHealth(100.f),
	//WeaponEquip
	bLeftKnuckleEquip(false), bRightKnuckleEquip(false), bHammerEquip(false), bLeftSwordEquip(false), bRightSwordEquip(false),
	bTwoHandedSwordEquip(false), bBowEquip(false),
	CurrentHeadHealth(100.f), CurrentBodyHealth(100.f), CurrentRightArmHealth(100.f), CurrentLeftArmHealth(100.f), CurrentRightLegHealth(100.f), CurrentLeftLegHealth(100.f),
	CurrentHealth(600.f)
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 45.f;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		Shinbi_Mesh(TEXT("SkeletalMesh'/Game/Retarget/Meshes/Shinbi_NoWeapon.Shinbi_NoWeapon'"));
	if (Shinbi_Mesh.Succeeded())
		GetMesh()->SetSkeletalMesh(Shinbi_Mesh.Object);


	if (PlayerAnim == nullptr)
		PlayerAnim = Cast<UMKKS_PlayerAnimInstance>(GetMesh()->GetAnimInstance());

	FName FirstPersonCameraSocket(TEXT("FirstPersonCameraSocket"));
	FName ThirdPersonCameraSocket(TEXT("ThirdPersonCameraSocket"));
	//LeftHandSocket = GetMesh()->GetSocketByName(FName("hand_l_Socket"));
	//RightHandSocket = GetMesh()->GetSocketByName(FName("hand_r_Socket"));
	FirstPersonFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonFollowCamera"));
	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));
	ThirdPersonFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonFollowCamera"));
	ThirdPersonCameraBoom->TargetArmLength = 150.f;
	ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);
	ThirdPersonFollowCamera->SetupAttachment(ThirdPersonCameraBoom, USpringArmComponent::SocketName);
	FirstPersonFollowCamera->SetupAttachment(GetMesh(), FirstPersonCameraSocket);
	FirstPersonFollowCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonFollowCamera->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f); // ...at this rotation rate

	bUseControllerRotationYaw = true;
	FirstPersonFollowCamera->bUsePawnControlRotation = true;
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	HeadDamageBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadDamageBox"));
	BodyDamageBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyDamageBox"));
	RightArmDamageBox1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmDamageBox1"));
	RightArmDamageBox2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmDamageBox2"));
	LeftArmDamageBox1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmDamageBox1"));
	LeftArmDamageBox2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmDamageBox2"));
	RightLegDamageBox1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegDamageBox1"));
	RightLegDamageBox2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegDamageBox2"));
	RightLegDamageBox3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegDamageBox3"));
	LeftLegDamageBox1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegDamageBox1"));
	LeftLegDamageBox2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegDamageBox2"));
	LeftLegDamageBox3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegDamageBox3"));

	FName HeadDamageSocket(TEXT("HeadDamageSocket"));
	FName BodyDamageSocket(TEXT("BodyDamageSocket"));
	FName RightArmDamageSocket1(TEXT("RightArmDamageSocket1"));
	FName RightArmDamageSocket2(TEXT("RightArmDamageSocket2"));
	FName LeftArmDamageSocket1(TEXT("LeftArmDamageSocket1"));
	FName LeftArmDamageSocket2(TEXT("LeftArmDamageSocket2"));
	FName RightLegDamageSocket1(TEXT("RightLegDamageSocket1"));
	FName RightLegDamageSocket2(TEXT("RightLegDamageSocket2"));
	FName RightLegDamageSocket3(TEXT("RightLegDamageSocket3"));
	FName LeftLegDamageSocket1(TEXT("LeftLegDamageSocket1"));
	FName LeftLegDamageSocket2(TEXT("LeftLegDamageSocket2"));
	FName LeftLegDamageSocket3(TEXT("LeftLegDamageSocket3"));


	HeadDamageBox->SetupAttachment(GetMesh(), HeadDamageSocket);
	BodyDamageBox->SetupAttachment(GetMesh(), BodyDamageSocket);
	RightArmDamageBox1->SetupAttachment(GetMesh(), RightArmDamageSocket1);
	RightArmDamageBox2->SetupAttachment(GetMesh(), RightArmDamageSocket2);
	LeftArmDamageBox1->SetupAttachment(GetMesh(), LeftArmDamageSocket1);
	LeftArmDamageBox2->SetupAttachment(GetMesh(), LeftArmDamageSocket2);
	RightLegDamageBox1->SetupAttachment(GetMesh(), RightLegDamageSocket1);
	RightLegDamageBox2->SetupAttachment(GetMesh(), RightLegDamageSocket2);
	RightLegDamageBox3->SetupAttachment(GetMesh(), RightLegDamageSocket3);
	LeftLegDamageBox1->SetupAttachment(GetMesh(), LeftLegDamageSocket1);
	LeftLegDamageBox2->SetupAttachment(GetMesh(), LeftLegDamageSocket2);
	LeftLegDamageBox3->SetupAttachment(GetMesh(), LeftLegDamageSocket3);
}

void ATeam_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATeam_ProjectCharacter::DecreaseFoodWater, FoodWaterDrainRate, true, 6.f);
}

void ATeam_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InputTimeCheck();
	Stamina(DeltaTime);
	BowAiming(DeltaTime);
	BarWidget();
}

void ATeam_ProjectCharacter::LeftHand()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginLeftHand"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bIsDead)
		return;

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("Q")))
	{
		if ((LeftEquippedWeapon != nullptr))
		{
			if (LeftEquippedWeapon->GetTwoHandedEquip() || LeftEquippedWeapon->GetBowEquip())
			{
				if ((AttachedWeapon == nullptr))
					AttachWeaponBack();
			}
			else if (LeftEquippedWeapon->GetOneHandedEquip())
			{

			}
		}
		else if (LeftEquippedWeapon == nullptr)
		{
			if (AttachedWeapon != nullptr)
				DetachWeaponBack();
		}

		return;
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("E"))
		&& ((LeftEquippedWeapon != nullptr) || (LeftEquippedItem != nullptr)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket isnt null"));
		ReleaseActor();
		return;
	}
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("E")))
	{
		if (TwoHandedCombatGrab())
		{
			return;
		}
		GrabActor();
		return;
	}

	if (CurrentStanceMode == EStance::ES_Default)
	{
		if (TwoHandedCombatCheck())
		{
			return;
		}
		CurrentStanceMode = EStance::ES_Combat;
		bCombatState = true;
		GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
		return;
	}


	if (!this->GetCharacterMovement()->IsFalling())
	{
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			bLeftHandAction = false;
		}
		else
		{
			if (AnimInstance)
			{
				bDoAttacking = true;
				UE_LOG(LogTemp, Warning, TEXT("DoAttacking"));

				if (RightEquippedWeapon == nullptr && RightEquippedItem == nullptr)
				{
					if (bTwoHandedSwordEquip)
					{
						AnimInstance->Montage_Play(LeftTwoHandedSwordAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
					else if (bHammerEquip)
					{
						AnimInstance->Montage_Play(LeftHammerAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
					else if (bBowEquip)
					{
						AnimInstance->Montage_Play(BowAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
				}
				if (bLeftSwordEquip)
				{
					AnimInstance->Montage_Play(LeftSwordAttackMontage);
					UE_LOG(LogTemp, Warning, TEXT("Sword"));
				}
				else if ((bLeftKnuckleEquip || (bLeftWeaponEquip == false)))
				{
					if (bRightWeaponEquip && (bHammerEquip || bTwoHandedSwordEquip))
					{
						bTwohandedKick = true;
						AnimInstance->Montage_Play(RTwoHandedKickMontage);
					}
					else
					{
						AnimInstance->Montage_Play(LeftPunchingMontage);
						UE_LOG(LogTemp, Warning, TEXT("Punch"));
					}
				}

			}
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("EndALeftHand"));
	bLeftHandAction = false;
}

void ATeam_ProjectCharacter::RightHand()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginRightHand"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (bIsDead)
		return;

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("Q")))
	{
		if ((RightEquippedWeapon != nullptr))
		{
			if (RightEquippedWeapon->GetTwoHandedEquip() || RightEquippedWeapon->GetBowEquip())
			{
				if ((AttachedWeapon == nullptr))
					AttachWeaponBack();
			}
			else if (LeftEquippedWeapon->GetOneHandedEquip())
			{

			}
		}
		else if (RightEquippedWeapon == nullptr)
		{
			if (AttachedWeapon != nullptr)
				DetachWeaponBack();
		}

		return;
	}

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("E"))
		&& ((RightEquippedWeapon != nullptr) || (RightEquippedItem != nullptr)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket isnt null"));
		ReleaseActor();
		return;
	}
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(FKey("E")))
	{
		if (TwoHandedCombatGrab())
		{
			return;
		}
		GrabActor();
		return;
	}

	if (CurrentStanceMode == EStance::ES_Default)
	{
		if (TwoHandedCombatCheck())
		{
			return;
		}
		CurrentStanceMode = EStance::ES_Combat;
		UE_LOG(LogTemp, Warning, TEXT("Change"));
		bCombatState = true;
		GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
		return;
	}

	if (!this->GetCharacterMovement()->IsFalling())
	{
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			bRightHandAction = true;
		}
		else
		{
			if (AnimInstance)
			{

				bDoAttacking = true;
				UE_LOG(LogTemp, Warning, TEXT("DoAttacking"));

				if (LeftEquippedWeapon == nullptr && LeftEquippedItem == nullptr)
				{
					if (bTwoHandedSwordEquip)
					{
						AnimInstance->Montage_Play(RightTwoHandedSwordAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
					else if (bHammerEquip)
					{
						AnimInstance->Montage_Play(RightHammerAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
					else if (bBowEquip)
					{
						AnimInstance->Montage_Play(BowAttackMontage);
						UE_LOG(LogTemp, Warning, TEXT("TwoHanded"));
					}
				}
				if (bRightSwordEquip)
				{
					AnimInstance->Montage_Play(RightSwordAttackMontage);
					UE_LOG(LogTemp, Warning, TEXT("Sword"));
				}
				else if ((bRightKnuckleEquip || (bRightWeaponEquip == false)))
				{
					if (bLeftWeaponEquip && (bHammerEquip || bTwoHandedSwordEquip))
					{
						bTwohandedKick = true;
						AnimInstance->Montage_Play(LTwoHandedKickMontage);
						//bTwohandedKick = false;
					}
					else
					{
						AnimInstance->Montage_Play(RightPunchingMontage);
						UE_LOG(LogTemp, Warning, TEXT("Punch"));
					}
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("EndARightHand"));
	bRightHandAction = false;
}

void ATeam_ProjectCharacter::LeftHandEnd()
{
	bDoAttacking = false;
	bTwohandedKick = false;
}

void ATeam_ProjectCharacter::RightHandEnd()
{
	bDoAttacking = false;
	bTwohandedKick = false;
}

void ATeam_ProjectCharacter::InputTimeCheck()
{
	PCInputTime_Stand = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputKeyTimeDown(FKey("X")) * 10.0f;
	PCInputTime_Interact = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputKeyTimeDown(FKey("E")) * 10.0f;
}

void ATeam_ProjectCharacter::CameraOption()
{
	if (bIsDead)
	{
		bUseControllerRotationYaw = false;
		UE_LOG(LogTemp, Warning, TEXT("gojung_A_nim"));
	}
	else if (!bLayingDown)
	{
		bUseControllerRotationYaw = true;
		UE_LOG(LogTemp, Warning, TEXT("gojung"));
	}
	else if (bLayingDown)
	{
		bUseControllerRotationYaw = false;
		UE_LOG(LogTemp, Warning, TEXT("gojung_A_nim"));
	}
}

void ATeam_ProjectCharacter::BlockModeAim()
{
}

void ATeam_ProjectCharacter::BowAiming(float DeltaTime)
{

	if (bAiming)
	{
		//bAiming = true;
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		//bAiming = false;
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	if (CurrentViewMode == EViewType::EVT_FirstPerson)
	{
		GetFirstPersonFollowCamera()->SetFieldOfView(CameraCurrentFOV);
	}
}

void ATeam_ProjectCharacter::BarWidget()
{
	if (IsLocallyControlled() && PlayerWidgetClass)
	{
		AMKKS_PlayerController* FPC = GetController<AMKKS_PlayerController>();
		check(FPC);
		PlayerWidget = CreateWidget<UBarWidget>(FPC, PlayerWidgetClass);
		check(PlayerWidget);
		PlayerWidget->AddToViewport();
		PlayerWidget->SetHealth(CurrentHealth, MaxHealth);
	}
}

void ATeam_ProjectCharacter::SetViewType(EViewType ViewType)
{

	switch (ViewType)
	{
	case EViewType::EVT_ThirdPerson:
		FirstPersonFollowCamera->Deactivate();
		ThirdPersonFollowCamera->Activate();
		CameraOption();
		break;

	case EViewType::EVT_FirstPerson:
		FirstPersonFollowCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
		FirstPersonFollowCamera->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
		ThirdPersonFollowCamera->Deactivate();
		FirstPersonFollowCamera->Activate();
		CameraOption();
		break;
	}
}

void ATeam_ProjectCharacter::ViewChange()
{
	if (!bIsDead)
	{
		if (CurrentViewMode == EViewType::EVT_ThirdPerson)
		{
			CurrentViewMode = EViewType::EVT_FirstPerson;
			SetViewType(CurrentViewMode);
			UE_LOG(LogTemp, Warning, TEXT("ViewChange"));
		}
		else
		{
			CurrentViewMode = EViewType::EVT_ThirdPerson;
			SetViewType(CurrentViewMode);
		}
	}
}

void ATeam_ProjectCharacter::StanceChange()
{
	if (!bIsDead)
	{
		if (CurrentStanceMode == EStance::ES_Default)
		{
			CurrentStanceMode = EStance::ES_Combat;
			SetStanceType(CurrentStanceMode);


			UE_LOG(LogTemp, Warning, TEXT("StanceChanged"));
		}
		else
		{
			CurrentStanceMode = EStance::ES_Default;

			SetStanceType(CurrentStanceMode);
		}
	}
}

void ATeam_ProjectCharacter::SetStanceType(EStance StanceType)
{
	switch (StanceType)
	{
	case EStance::ES_Default:
		bCombatState = false;

		if (CurrentStanding == EStanding::ESD_Standing)
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
			CameraOption();
		}
		if (CurrentStanding == EStanding::ESD_Crouching)
		{
			GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
			CameraOption();
		}
		if (CurrentStanding == EStanding::ESD_Sitting || CurrentStanding == EStanding::ESD_LayingDown)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			CameraOption();
		}
		break;

	case EStance::ES_Combat:
		bCombatState = true;
		if (CurrentStanding == EStanding::ESD_Standing || CurrentStanding == EStanding::ESD_Crouching)
		{
			GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
			CameraOption();
		}
		if (CurrentStanding == EStanding::ESD_Sitting || CurrentStanding == EStanding::ESD_LayingDown)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			CameraOption();
		}
		break;
	}
}

void ATeam_ProjectCharacter::SetStanding(EStanding StandingType)
{
	switch (StandingType)
	{
	case EStanding::ESD_Standing:
		bLayingDown = false;
		bCrouching = false;
		bSprint = false;
		bIsDead = false;
		if (CurrentStanceMode == EStance::ES_Default)
		{
			CameraOption();
			GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		}
		if (CurrentStanceMode == EStance::ES_Combat)
		{
			CameraOption();
			GetCharacterMovement()->MaxWalkSpeed = CombatSpeed;
		}
		ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
		break;

	case EStanding::ESD_Crouching:
		bCrouching = true;
		bLayingDown = false;
		bSprint = false;
		bIsDead = false;
		if (CurrentStanceMode == EStance::ES_Combat || CurrentStanceMode == EStance::ES_Default)
		{
			CameraOption();
			GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		}
		ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		break;

	case EStanding::ESD_Sprinting:
		bCrouching = false;
		bLayingDown = false;
		bSprint = true;
		bIsDead = false;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;

	case EStanding::ESD_LayingDown:
		bCrouching = false;
		bLayingDown = true;
		bSprint = false;
		bIsDead = false;
		if (CurrentStanceMode == EStance::ES_Combat || CurrentStanceMode == EStance::ES_Default)
		{
			CameraOption();
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		}
		ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
		break;

	case EStanding::ESD_Dead:
		bCrouching = false;
		bLayingDown = false;
		bSprint = false;
		bIsDead = true;
		CameraOption();
		ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
		break;
	}
}

void ATeam_ProjectCharacter::StandingChange()
{
	if (!bIsDead)
	{
		if (CurrentStanding == EStanding::ESD_Standing)
		{
			if (inputTime <= PCInputTime_Stand)
			{
				CurrentStanding = EStanding::ESD_LayingDown;
				SetStanding(CurrentStanding);
				UE_LOG(LogTemp, Warning, TEXT("LyingDown"));
				return;
			}

			CurrentStanding = EStanding::ESD_Crouching;
			SetStanding(CurrentStanding);
			UE_LOG(LogTemp, Warning, TEXT("Sitting"));
		}
		else if (CurrentStanding == EStanding::ESD_Crouching)
		{
			if (inputTime <= PCInputTime_Stand)
			{
				CurrentStanding = EStanding::ESD_LayingDown;
				SetStanding(CurrentStanding);
				UE_LOG(LogTemp, Warning, TEXT("LyingDown"));
				return;
			}

			CurrentStanding = EStanding::ESD_Standing;
			SetStanding(CurrentStanding);
			UE_LOG(LogTemp, Warning, TEXT("Standing"));
		}
		else
		{
			if (inputTime <= PCInputTime_Stand)
			{

				CurrentStanding = EStanding::ESD_Standing;
				SetStanding(CurrentStanding);
				UE_LOG(LogTemp, Warning, TEXT("Standing"));
				return;
			}

			CurrentStanding = EStanding::ESD_Crouching;
			SetStanding(CurrentStanding);
			UE_LOG(LogTemp, Warning, TEXT("Sitting"));
		}
	}
}


void ATeam_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATeam_ProjectCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATeam_ProjectCharacter::StopJumping);
	PlayerInputComponent->BindAction("LeftHand", IE_Pressed, this, &ATeam_ProjectCharacter::LeftHand);
	PlayerInputComponent->BindAction("RightHand", IE_Pressed, this, &ATeam_ProjectCharacter::RightHand);;
	PlayerInputComponent->BindAction("LeftHand", IE_Released, this, &ATeam_ProjectCharacter::LeftHandEnd);
	PlayerInputComponent->BindAction("RightHand", IE_Released, this, &ATeam_ProjectCharacter::RightHandEnd);;
	PlayerInputComponent->BindAction("ViewChange", IE_Pressed, this, &ATeam_ProjectCharacter::ViewChange);
	PlayerInputComponent->BindAction("StanceChange", IE_Pressed, this, &ATeam_ProjectCharacter::StanceChange);
	PlayerInputComponent->BindAction("StandingChange", IE_Released, this, &ATeam_ProjectCharacter::StandingChange);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATeam_ProjectCharacter::Interact);
	PlayerInputComponent->BindAction("AddToInventory", IE_Pressed, this, &ATeam_ProjectCharacter::AddToInventory);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ATeam_ProjectCharacter::OpenInventory);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATeam_ProjectCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATeam_ProjectCharacter::SprintEnd);
	PlayerInputComponent->BindAction("BlockMode/Aim", IE_Axis, this, &ATeam_ProjectCharacter::BlockModeAim);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATeam_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATeam_ProjectCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATeam_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATeam_ProjectCharacter::LookUpAtRate);
}

void ATeam_ProjectCharacter::Interact()
{
	if (bIsDead)
		return;

	if (inputTime <= PCInputTime_Interact)
	{

	}
}

void ATeam_ProjectCharacter::GrabActor()
{
	FVector Start = ThirdPersonFollowCamera->GetComponentLocation();
	// Distance to Interact = 500.0f;
	FVector End = Start + ThirdPersonFollowCamera->GetComponentRotation().Vector() * 500.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bResult = GetWorld()->LineTraceSingleByChannel
	(
		HitResult,
		Start, End,
		ECollisionChannel::ECC_Visibility,
		Params
	);


	if (bResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor"));
		TraceItem = Cast<AItem>(HitResult.GetActor());
		TraceWeapon = Cast<AWeapon>(HitResult.GetActor());
		FName HandSocket(TEXT("hand_r_Socket"));

		if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			//LeftHand
			const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName(FName("hand_l_Socket"));

			if (TraceWeapon)
			{
				UE_LOG(LogTemp, Warning, TEXT("This is LeftWeapon"));
				TraceWeapon->SetItemState(EItemState::EIS_Equip);
				//TraceWeapon->GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//이게 되야 캐릭터가 템이랑 메쉬가 블록처리되지 않음(SetItemState 에 들어있음)
				if (TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetFistWeapon())
				{
					const USkeletalMeshSocket* FistSocket = GetMesh()->GetSocketByName(FName("hand_l_FSocket"));
					FistSocket->AttachActor(TraceWeapon, GetMesh());
					bLeftKnuckleEquip = true;
					//GrabbedTRCheck();
				}
				else if ((TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetSwordWeapon()) ||
					(TraceWeapon->GetTwoHandedEquip() && TraceWeapon->GetHammerWeapon()))
				{
					LeftHandSocket->AttachActor(TraceWeapon, GetMesh());
					if (TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetSwordWeapon())
					{
						bLeftSwordEquip = true;
						//GrabbedTRCheck();
					}
					else
					{
						bHammerEquip = true;
						//GrabbedRightActorLTCheck();
					}
				}
				else if (TraceWeapon->GetTwoHandedEquip() && TraceWeapon->GetSwordWeapon())
				{
					const USkeletalMeshSocket* TSSocket = GetMesh()->GetSocketByName(FName("hand_l_TSSocket"));
					TSSocket->AttachActor(TraceWeapon, GetMesh());
					bTwoHandedSwordEquip = true;
					//GrabbedRightActorLTCheck();
				}
				else if (TraceWeapon->GetBowEquip() && TraceWeapon->GetBowWeapon())
				{
					const USkeletalMeshSocket* BowSocket = GetMesh()->GetSocketByName(FName("hand_l_BSocket"));
					BowSocket->AttachActor(TraceWeapon, GetMesh());
					bBowEquip = true;
					//GrabbedTRCheck();
				}
				LeftEquippedWeapon = TraceWeapon;
				bLeftWeaponEquip = true;
				bWeaponIsLeftHand = true;
			}
			else if (TraceItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("This is LeftItem"));
				TraceItem->SetItemState(EItemState::EIS_Equip);
				LeftHandSocket->AttachActor(TraceItem, GetMesh());
				LeftEquippedItem = TraceItem;
				bLeftItemEquip = true;
				//GrabbedTRCheck();
			}


		}
		else if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::RightMouseButton))
		{
			//RightHand
			const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(FName("hand_r_Socket"));

			if (TraceWeapon)
			{
				UE_LOG(LogTemp, Warning, TEXT("This is RightWeapon"));
				TraceWeapon->SetItemState(EItemState::EIS_Equip);
				if (TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetFistWeapon())
				{
					const USkeletalMeshSocket* FistSocket = GetMesh()->GetSocketByName(FName("hand_r_FSocket"));
					FistSocket->AttachActor(TraceWeapon, GetMesh());
					bRightKnuckleEquip = true;
					//GrabbedTLCheck();
				}
				else if ((TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetSwordWeapon()) ||
					(TraceWeapon->GetTwoHandedEquip() && TraceWeapon->GetHammerWeapon()))
				{
					RightHandSocket->AttachActor(TraceWeapon, GetMesh());
					if (TraceWeapon->GetOneHandedEquip() && TraceWeapon->GetSwordWeapon())
					{
						bRightSwordEquip = true;
						//GrabbedTLCheck();
					}
					else
					{
						bHammerEquip = true;
						//GrabbedLeftActorRTCheck();
					}
				}
				else if (TraceWeapon->GetTwoHandedEquip() && TraceWeapon->GetSwordWeapon())
				{
					const USkeletalMeshSocket* TSSocket = GetMesh()->GetSocketByName(FName("hand_r_TSSocket"));
					TSSocket->AttachActor(TraceWeapon, GetMesh());
					bTwoHandedSwordEquip = true;
					//GrabbedLeftActorRTCheck();
				}
				else if (TraceWeapon->GetBowEquip() && TraceWeapon->GetBowWeapon())
				{
					const USkeletalMeshSocket* BowSocket = GetMesh()->GetSocketByName(FName("hand_r_BSocket"));
					BowSocket->AttachActor(TraceWeapon, GetMesh());
					bBowEquip = true;
				}
				RightEquippedWeapon = TraceWeapon;
				bRightWeaponEquip = true;
				bWeaponIsLeftHand = true;
				//GrabbedTLCheck();
			}
			else if (TraceItem)
			{
				UE_LOG(LogTemp, Warning, TEXT("This is RightItem"));
				TraceItem->SetItemState(EItemState::EIS_Equip);
				RightHandSocket->AttachActor(TraceItem, GetMesh());
				RightEquippedItem = TraceItem;
				bRightItemEquip = true;
				//GrabbedTLCheck();
			}
		}
	}
}

void ATeam_ProjectCharacter::ReleaseActor()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		//LeftHand

		if (LeftEquippedWeapon)
		{
			LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
			LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

			if (LeftEquippedWeapon->GetTwoHandedEquip())
			{
				bRightWeaponEquip = false;
				bWeaponIsRightHand = false;
			}

			bLeftWeaponEquip = false;
			bWeaponIsLeftHand = false;
			bLeftKnuckleEquip = false;
			bLeftSwordEquip = false;
			bTwoHandedSwordEquip = false;
			bHammerEquip = false;
			LeftEquippedWeapon = nullptr;

		}
		else if (LeftEquippedItem)
		{
			LeftEquippedItem->DetachFromActor(DetachmentTransformRules);
			LeftEquippedItem->SetItemState(EItemState::EIS_Ground);
			LeftEquippedItem = nullptr;
			bLeftItemEquip = false;

		}

	}
	else if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::RightMouseButton))
	{
		//RightHand

		if (RightEquippedWeapon)
		{
			RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
			RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

			if (RightEquippedWeapon->GetTwoHandedEquip())
			{
				bLeftWeaponEquip = false;
				bWeaponIsLeftHand = false;
			}

			bRightWeaponEquip = false;
			bWeaponIsRightHand = false;
			bRightKnuckleEquip = false;
			bRightSwordEquip = false;
			bTwoHandedSwordEquip = false;
			bHammerEquip = false;
			RightEquippedWeapon = nullptr;

		}
		else if (RightEquippedItem)
		{
			RightEquippedItem->DetachFromActor(DetachmentTransformRules);
			RightEquippedItem->SetItemState(EItemState::EIS_Ground);
			RightEquippedItem = nullptr;
			bRightItemEquip = false;
		}
	}
}

void ATeam_ProjectCharacter::GrabbedRightActorLTCheck()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	if (RightEquippedItem != nullptr)
	{
		RightEquippedItem->DetachFromActor(DetachmentTransformRules);
		RightEquippedItem->SetItemState(EItemState::EIS_Ground);
		RightEquippedItem = nullptr;
		bRightItemEquip = false;
	}
	else if (RightEquippedWeapon != nullptr)
	{
		RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
		RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

		bRightWeaponEquip = false;
		bWeaponIsRightHand = false;
		bRightKnuckleEquip = false;
		bRightSwordEquip = false;

		RightEquippedWeapon = nullptr;
	}
}

void ATeam_ProjectCharacter::GrabbedLeftActorRTCheck()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	if (LeftEquippedWeapon)
	{
		LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
		LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

		bLeftWeaponEquip = false;
		bWeaponIsLeftHand = false;
		bLeftKnuckleEquip = false;
		bLeftSwordEquip = false;

		LeftEquippedWeapon = nullptr;

	}
	else if (LeftEquippedItem)
	{
		LeftEquippedItem->DetachFromActor(DetachmentTransformRules);
		LeftEquippedItem->SetItemState(EItemState::EIS_Ground);
		LeftEquippedItem = nullptr;
		bLeftItemEquip = false;

	}
}

void ATeam_ProjectCharacter::GrabbedTLCheck()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

	if (LeftEquippedWeapon != nullptr)
	{
		if (LeftEquippedWeapon->GetTwoHandedEquip())
		{
			LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
			LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

			bHammerEquip = false;
			bTwoHandedSwordEquip = false;

			LeftEquippedWeapon = nullptr;
		}
	}
}

void ATeam_ProjectCharacter::GrabbedTRCheck()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

	if (RightEquippedWeapon != nullptr)
	{
		if (RightEquippedWeapon->GetTwoHandedEquip())
		{
			RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
			RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

			bHammerEquip = false;
			bTwoHandedSwordEquip = false;

			RightEquippedWeapon = nullptr;
		}
	}
}

bool ATeam_ProjectCharacter::TwoHandedCombatCheck()
{
	if (RightEquippedWeapon != nullptr)
	{
		if (RightEquippedWeapon->GetTwoHandedEquip() || RightEquippedWeapon->GetBowEquip())
		{
			if (LeftEquippedItem != nullptr || LeftEquippedWeapon != nullptr)
			{
				return true;
			}
		}
	}
	else if (LeftEquippedWeapon != nullptr)
	{
		if (LeftEquippedWeapon->GetTwoHandedEquip() || LeftEquippedWeapon->GetBowEquip())
		{
			if (RightEquippedItem != nullptr || RightEquippedWeapon != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

bool ATeam_ProjectCharacter::TwoHandedCombatGrab()
{
	if (LeftEquippedWeapon != nullptr)
	{
		if (LeftEquippedWeapon->GetTwoHandedEquip())
		{
			if (CurrentStanceMode == EStance::ES_Combat)
			{
				return true;
			}
		}
	}
	else if (RightEquippedWeapon != nullptr)
	{
		if (RightEquippedWeapon->GetTwoHandedEquip())
		{
			if (CurrentStanceMode == EStance::ES_Combat)
			{
				return true;
			}
		}
	}
	return false;
}

void ATeam_ProjectCharacter::AttachWeaponBack()
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		if (LeftEquippedWeapon != nullptr)
		{
			if (LeftEquippedWeapon->GetTwoHandedEquip() || LeftEquippedWeapon->GetBowEquip())
			{
				UE_LOG(LogTemp, Warning, TEXT("Twohanded or Bow"));
				FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

				if (LeftEquippedWeapon->GetTwoHandedEquip() && LeftEquippedWeapon->GetSwordWeapon())
				{
					UE_LOG(LogTemp, Warning, TEXT("It's Twohanded"));
					const USkeletalMeshSocket* TSBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_SWeaponSocket"));

					LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bHammerEquip = false;
					bTwoHandedSwordEquip = false;
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					TSBackSocket->AttachActor(LeftEquippedWeapon, GetMesh());

				}
				else if (LeftEquippedWeapon->GetTwoHandedEquip() && LeftEquippedWeapon->GetHammerWeapon())
				{
					const USkeletalMeshSocket* THBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_HWeaponSocket"));

					LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bHammerEquip = false;
					bTwoHandedSwordEquip = false;
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					THBackSocket->AttachActor(LeftEquippedWeapon, GetMesh());

				}
				else if (LeftEquippedWeapon->GetBowEquip())
				{
					const USkeletalMeshSocket* BBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_BWeaponSocket"));

					LeftEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bBowEquip = false;
					LeftEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					BBackSocket->AttachActor(LeftEquippedWeapon, GetMesh());

				}
				AttachedWeapon = LeftEquippedWeapon;
				LeftEquippedWeapon = nullptr;
				bLeftWeaponEquip = false;
			}
			else if (LeftEquippedWeapon->GetOneHandedEquip())
			{

			}
		}
		else
		{
			return;
		}
	}
	else if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::RightMouseButton))
	{
		if (RightEquippedWeapon != nullptr)
		{

			if (RightEquippedWeapon->GetTwoHandedEquip() || RightEquippedWeapon->GetBowEquip())
			{
				UE_LOG(LogTemp, Warning, TEXT("Twohanded or Bow"));
				FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

				if (RightEquippedWeapon->GetTwoHandedEquip() && RightEquippedWeapon->GetSwordWeapon())
				{
					UE_LOG(LogTemp, Warning, TEXT("It's Twohanded"));
					const USkeletalMeshSocket* TSBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_SWeaponSocket"));

					RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bHammerEquip = false;
					bTwoHandedSwordEquip = false;
					RightEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					TSBackSocket->AttachActor(RightEquippedWeapon, GetMesh());

				}
				else if (RightEquippedWeapon->GetTwoHandedEquip() && RightEquippedWeapon->GetHammerWeapon())
				{
					const USkeletalMeshSocket* THBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_HWeaponSocket"));

					RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bHammerEquip = false;
					bTwoHandedSwordEquip = false;
					RightEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					THBackSocket->AttachActor(RightEquippedWeapon, GetMesh());

				}
				else if (RightEquippedWeapon->GetBowEquip())
				{
					const USkeletalMeshSocket* BBackSocket = GetMesh()->GetSocketByName(FName("UnEquip_BWeaponSocket"));

					RightEquippedWeapon->DetachFromActor(DetachmentTransformRules);
					RightEquippedWeapon->SetItemState(EItemState::EIS_Ground);

					bBowEquip = false;
					RightEquippedWeapon->SetItemState(EItemState::EIS_Equip);
					BBackSocket->AttachActor(RightEquippedWeapon, GetMesh());

				}
				AttachedWeapon = RightEquippedWeapon;
				RightEquippedWeapon = nullptr;
				bRightWeaponEquip = false;
			}
			else if (RightEquippedWeapon->GetOneHandedEquip())
			{

			}

		}
		else
		{
			return;
		}
	}
}

void ATeam_ProjectCharacter::DetachWeaponBack()
{
	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		if (LeftEquippedWeapon == nullptr)
		{
			const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName(FName("hand_l_Socket"));


			if (AttachedWeapon != nullptr)
			{
				if (AttachedWeapon->GetTwoHandedEquip() || AttachedWeapon->GetBowEquip())
				{
					if (AttachedWeapon->GetTwoHandedEquip() && AttachedWeapon->GetSwordWeapon())
					{
						const USkeletalMeshSocket* TSSocket = GetMesh()->GetSocketByName(FName("hand_l_TSSocket"));

						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bTwoHandedSwordEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						TSSocket->AttachActor(AttachedWeapon, GetMesh());
					}
					else if (AttachedWeapon->GetTwoHandedEquip() && AttachedWeapon->GetHammerWeapon())
					{
						const USkeletalMeshSocket* THSocket = GetMesh()->GetSocketByName(FName("hand_l_Socket"));
						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bHammerEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						THSocket->AttachActor(AttachedWeapon, GetMesh());
					}
					else if (AttachedWeapon->GetBowEquip())
					{
						const USkeletalMeshSocket* BowSocket = GetMesh()->GetSocketByName(FName("hand_l_BSocket"));
						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bBowEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						BowSocket->AttachActor(AttachedWeapon, GetMesh());
					}

					LeftEquippedWeapon = AttachedWeapon;
					AttachedWeapon = nullptr;
					bLeftWeaponEquip = true;
					bWeaponIsLeftHand = true;
				}
				else if (AttachedWeapon->GetOneHandedEquip())
				{

				}
			}
		}
		else if (LeftEquippedWeapon != nullptr)
		{
			return;
		}

	}
	else if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::RightMouseButton))
	{
		if (RightEquippedWeapon == nullptr)
		{
			if (AttachedWeapon != nullptr)
			{
				if (AttachedWeapon->GetTwoHandedEquip() || AttachedWeapon->GetBowEquip())
				{
					if (AttachedWeapon->GetTwoHandedEquip() && AttachedWeapon->GetSwordWeapon())
					{
						const USkeletalMeshSocket* TSSocket = GetMesh()->GetSocketByName(FName("hand_r_TSSocket"));

						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bTwoHandedSwordEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						TSSocket->AttachActor(AttachedWeapon, GetMesh());
					}
					else if (AttachedWeapon->GetTwoHandedEquip() && AttachedWeapon->GetHammerWeapon())
					{
						const USkeletalMeshSocket* THSocket = GetMesh()->GetSocketByName(FName("hand_r_Socket"));
						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bHammerEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						THSocket->AttachActor(AttachedWeapon, GetMesh());
					}
					else if (AttachedWeapon->GetBowEquip())
					{
						const USkeletalMeshSocket* BowSocket = GetMesh()->GetSocketByName(FName("hand_r_BSocket"));
						AttachedWeapon->DetachFromActor(DetachmentTransformRules);
						AttachedWeapon->SetItemState(EItemState::EIS_Ground);

						bBowEquip = true;
						AttachedWeapon->SetItemState(EItemState::EIS_Equip);
						BowSocket->AttachActor(AttachedWeapon, GetMesh());
					}

					RightEquippedWeapon = AttachedWeapon;
					AttachedWeapon = nullptr;
					bRightWeaponEquip = true;
					bWeaponIsRightHand = true;
				}
				else if (AttachedWeapon->GetOneHandedEquip())
				{

				}
			}
		}
		else if (RightEquippedWeapon != nullptr)
		{
			return;
		}
	}
}

void ATeam_ProjectCharacter::AddToInventory()
{
	PutActor();
}

void ATeam_ProjectCharacter::PutActor()
{
	FVector Start = ThirdPersonFollowCamera->GetComponentLocation();
	// Distance to Interact = 500.0f;
	FVector End = Start + ThirdPersonFollowCamera->GetComponentRotation().Vector() * 500.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR"));

		if (AActor* HitActor = HitResult.GetActor())
		{
		}
	}
}

void ATeam_ProjectCharacter::OpenInventory()
{
	if (bIsDead)
		return;

	if (!bIsInventoryOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Open"));

		PlayerInventoryPanel = CreateWidget<UInventoryPanel>(GetWorld(), PlayerInventoryPanelClass);
		check(PlayerInventoryPanel);
		PlayerInventoryPanel->AddToViewport();
		bIsInventoryOpen = true;
	}
	else
	{
		PlayerInventoryPanel->RemoveFromViewport();
		bIsInventoryOpen = false;
	}
}

void ATeam_ProjectCharacter::SprintStart()
{
	if ((GetInputAxisValue(TEXT("Move Right / Left")) == 0) && (GetInputAxisValue(TEXT("Move Forward / Backward")) > 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("We are now sprinting."));
		CurrentStanding = EStanding::ESD_Sprinting;
		SetStanding(CurrentStanding);
	}

}

void ATeam_ProjectCharacter::SprintEnd()
{
	if (bSprint == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("We have stopped sprinting."));
		CurrentStanding = EStanding::ESD_Standing;
		SetStanding(CurrentStanding);
	}
}

void ATeam_ProjectCharacter::Stamina(float DeltaTime)
{
	if (currentStamina <= 0)
	{
		SprintEnd();
	}

	if (bSprint)
	{
		currentStamina = FMath::FInterpConstantTo(currentStamina, 0.0f, DeltaTime, staminaSprintUsageRate);
	}

	else
	{
		if (currentStamina < MaxStamina)
		{
			currentStamina = FMath::FInterpConstantTo(currentStamina, MaxStamina, DeltaTime, staminaRechargeRate);
		}
	}
}

void ATeam_ProjectCharacter::UseItem(AItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); // BP event
	}
}

void ATeam_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATeam_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());

}

void ATeam_ProjectCharacter::Jump()
{
	if (!bIsDead && !bLayingDown)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ATeam_ProjectCharacter::StopJumping()
{
	bPressedJump = false;

	if (!bIsDead || !bLayingDown)
		ResetJumpState();
}


void ATeam_ProjectCharacter::MoveForward(float Value)
{
	if (!bIsDead)
	{

		if (bTwohandedKick)
			return;

		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			if (Value != 0.0f)
			{
				// add movement in that direction
				AddMovementInput(GetActorForwardVector(), Value);
			}
		}
		else
		{
			if ((Controller != nullptr) && (Value != 0.0f))
			{
				// find out which way is forward
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get forward vector
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				AddMovementInput(Direction, Value);
			}
		}
	}
}

void ATeam_ProjectCharacter::MoveRight(float Value)
{
	if (!bIsDead)
	{

		if (bTwohandedKick)
			return;

		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			if (Value != 0.0f)
			{
				// add movement in that direction
				AddMovementInput(GetActorRightVector(), Value);
			}
		}
		else
		{
			if ((Controller != nullptr) && (Value != 0.0f))
			{
				// find out which way is right
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get right vector 
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				// add movement in that direction
				AddMovementInput(Direction, Value);
			}
		}
	}
}

void ATeam_ProjectCharacter::DecreaseFoodWater()
{
	CurrentHungry = CurrentHungry - HungryRate;
	CurrentThirsty = CurrentThirsty - ThirstyRate;

	if (CurrentHungry <= 0 || CurrentThirsty <= 0)
	{
		CurrentStanding = EStanding::ESD_Dead;
		SetStanding(CurrentStanding);
	}
}