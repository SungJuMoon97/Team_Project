
#include "Team_ProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
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

ATeam_ProjectCharacter::ATeam_ProjectCharacter() :
	//if your View and Stance make a change
	CurrentViewMode(EViewType::EVT_FirstPerson), CurrentStanceMode(EStance::ES_Default),
	bCombatState(false), bSprint(false),bWeaponEquip(false),bAiming(false),
	//CurrentSpeed
	DefaultSpeed(500.0f), CrouchSpeed(200.0f), CombatSpeed(200.0f), SprintSpeed(800.0f),
	//if you HandAction Default Setting
	CurrentWeapon(EWeaponType::EWT_Fist), CurrentHandWeapon(EWeaponHand::EWH_Fist),
	PreviouslyEquippedWeapon(EWeaponHand::EWH_Fist), CompareWeapon(EWeaponHand::EWH_Fist),
	BareHandDamage(10), bLeftHandAction(false), bRightHandAction(false), bDoAttacking(false), bBlocking(false),
	//if Character Sitting or Lying or Standing
	CurrentStanding(EStanding::ESD_Standing),
	bSitting(false), bLayingDown(false), bCrouching(false),
	inputTime(2.0f),
	bIsInventoryOpen(false),
	//PlayerStat
	Health(100.f),MaxStamina(1.0f), MaxHungry(100.0f),MaxThirsty(100.0f),
	currentStamina(1.0f),staminaSprintUsageRate(0.05f),staminaRechargeRate(0.01f),
	CurrentHungry(100.0f), CurrentThirsty(100.f),HungryRate(0.5f),ThirstyRate(0.8f),
	FoodWaterDrainRate(10.0f),//배고픔목마름줄어드는시간
	//BowAiming Setting
	CameraCurrentFOV(90.0f), CameraDefaultFOV(90.0f),CameraZoomedFOV(50.0f),ZoomInterpSpeed(20.0f),
	bIsHoldingItem(false)
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

	bIsItem = MyItem->GetIsItem();
	bIsWeapon = MyWeapon->GetIsWeapon();
}

void ATeam_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATeam_ProjectCharacter::DecreaseFoodWater, FoodWaterDrainRate, true, 6.f);

	if (IsLocallyControlled() && PlayerWidgetClass)
	{
		AMKKS_PlayerController* FPC = GetController<AMKKS_PlayerController>();
		check(FPC);
		PlayerWidget = CreateWidget<UBarWidget>(FPC, PlayerWidgetClass);
		check(PlayerWidget);
		PlayerWidget->AddToViewport();
	}
}

void ATeam_ProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InputTimeCheck();
	Stamina(DeltaTime);
	BowAiming(DeltaTime);
	WeaponChangeCheck();
}

void ATeam_ProjectCharacter::LeftHand()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginLeftHand"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentStanceMode == EStance::ES_Default)
	{
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
			bLeftHandAction = false;
		}
		else
		{
			if (AnimInstance && LeftPunchingMontage && (CurrentWeapon == EWeaponType::EWT_Fist))
			{
				//PlayAnimMontage(LeftPunchingMontage, 1, NAME_None);


				AnimInstance->Montage_Play(LeftPunchingMontage);
				bDoAttacking = true;
				UE_LOG(LogTemp, Warning, TEXT("Punch"));
			}
		}

	}
	
	UE_LOG(LogTemp, Warning, TEXT("EndALeftHand"));
	bDoAttacking = false;
	bLeftHandAction = false;
}

void ATeam_ProjectCharacter::RightHand()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginRightHand"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentStanceMode == EStance::ES_Default)
	{
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
			if (AnimInstance && RightPunchingMontage && (CurrentWeapon == EWeaponType::EWT_Fist))
			{
				//PlayAnimMontage(LeftPunchingMontage, 1, NAME_None);

				if (CurrentHandWeapon == EWeaponHand::EWH_Fist)
				{
					AnimInstance->Montage_Play(RightPunchingMontage);
					bDoAttacking = true;
					UE_LOG(LogTemp, Warning, TEXT("Punch"));
				}
				
			}
			else if (CurrentHandWeapon == EWeaponHand::EWH_Bow)
			{
				bAiming = true;

			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("EndARightHand"));
	bDoAttacking = false;
	bRightHandAction = false;
}

void ATeam_ProjectCharacter::LeftHandEnd()
{
}

void ATeam_ProjectCharacter::RightHandEnd()
{
	bAiming = false;
}

void ATeam_ProjectCharacter::InputTimeCheck()
{
	PCInputTime_Stand = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputKeyTimeDown(FKey("X")) * 10.0f;
	PCInputTime_Interact = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputKeyTimeDown(FKey("E")) * 10.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("How long was the key pressed: %f"), PCInputTime_Stand));
}

void ATeam_ProjectCharacter::CameraOption()
{
	if (!bLayingDown)
	{
		bUseControllerRotationYaw = true;
		UE_LOG(LogTemp, Warning, TEXT("gojung"));
	}
	else// if (bLayingDown)
	{
		bUseControllerRotationYaw = false;
		UE_LOG(LogTemp, Warning, TEXT("gojung_Anim"));
	}
}

bool ATeam_ProjectCharacter::WeaponEquip()
{
	bWeaponEquip = true;
	return false;
}

bool ATeam_ProjectCharacter::WeaponSeparate()
{
	return false;
}

void ATeam_ProjectCharacter::BlockModeAim()
{
	//if (bWeaponEquip == true)
	//{
	//	if (CurrentHandWeapon == EWeaponHand::EWH_Bow)
	//	{
			if (CurrentViewMode == EViewType::EVT_FirstPerson)
			{
				
			}
			if (CurrentViewMode == EViewType::EVT_ThirdPerson)
			{
				ThirdPersonFollowCamera->Deactivate();
				FirstPersonFollowCamera->Activate();
			}
	//	}

	//	if (CurrentViewMode == EViewType::EVT_FirstPerson)
	//	{
	//		if (CurrentHandWeapon == EWeaponHand::EWH_OneHanded)
	//		{

	//		}
	//		else if (CurrentHandWeapon == EWeaponHand::EWH_TwoHanded)
	//		{

	//		}
	//		else if (CurrentHandWeapon == EWeaponHand::EWH_Fist)
	//		{

	//		}
	//	}
	//}
}

void ATeam_ProjectCharacter::BowAiming(float DeltaTime)
{
	//if (CurrentHandWeapon == EWeaponHand::EWH_Bow)
	//{
		/*float Value = GetInputAxisValue(TEXT("BlockMode/Aim"));
		if (Value > 1)
			Value = 1;
		if (Value < -1)
			Value = -1;*/

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
	//}
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			GetFirstPersonFollowCamera()->SetFieldOfView(CameraCurrentFOV);
		}
}


void ATeam_ProjectCharacter::WeaponChangeCheck()
{
	if (CompareWeapon != CurrentHandWeapon)
	{
		PreviouslyEquippedWeapon = CompareWeapon;
		CompareWeapon = CurrentHandWeapon;
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

void ATeam_ProjectCharacter::StanceChange()
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
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		break;

	case EStanding::ESD_LayingDown:
		bCrouching = false;
		bLayingDown = true;
		bSprint = false;
		if (CurrentStanceMode == EStance::ES_Combat || CurrentStanceMode == EStance::ES_Default)
		{
			CameraOption();
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
		}
		ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
		break;
	}
}

void ATeam_ProjectCharacter::StandingChange()
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


void ATeam_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LeftHand", IE_Pressed, this, &ATeam_ProjectCharacter::LeftHand);
	PlayerInputComponent->BindAction("RightHand", IE_Pressed, this, &ATeam_ProjectCharacter::RightHand);;
	PlayerInputComponent->BindAction("LeftHand",IE_Released, this, &ATeam_ProjectCharacter::LeftHandEnd);
	PlayerInputComponent->BindAction("RightHand", IE_Released, this, &ATeam_ProjectCharacter::RightHandEnd);;
	PlayerInputComponent->BindAction("ViewChange", IE_Pressed, this, &ATeam_ProjectCharacter::ViewChange);
	PlayerInputComponent->BindAction("StanceChange", IE_Pressed, this, &ATeam_ProjectCharacter::StanceChange);
	PlayerInputComponent->BindAction("StandingChange", IE_Released, this, &ATeam_ProjectCharacter::StandingChange);
	// Item Interact binding (Press E)
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATeam_ProjectCharacter::Interact);
	PlayerInputComponent->BindAction("AddToInventory", IE_Pressed, this, &ATeam_ProjectCharacter::AddToInventory);
	// AddToInventory binding (Press F)
	PlayerInputComponent->BindAction("AddToInventory", IE_Pressed, this, &ATeam_ProjectCharacter::AddToInventory);
	// OpenInventory binding (Press I)
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ATeam_ProjectCharacter::OpenInventory);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATeam_ProjectCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATeam_ProjectCharacter::SprintEnd);
	PlayerInputComponent->BindAction("BlockMode/Aim", IE_Axis, this, &ATeam_ProjectCharacter::BlockModeAim);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATeam_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATeam_ProjectCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATeam_ProjectCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ATeam_ProjectCharacter::TurnRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATeam_ProjectCharacter::LookUpAtRate);
}

void ATeam_ProjectCharacter::Interact()
{
	if (HeldActor)
	{
		if (bIsItem)
		{
			if (bIsWeapon)
			{

			}
		}

		ReleaseActor();
	}
	else
	{
		if (bIsItem)
		{
			if (bIsWeapon)
			{

			}
		}

		GrabActor();
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

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR"));

		if (AActor* HitActor = HitResult.GetActor())
		{
			if (IPickup_Interface* Interface = Cast<IPickup_Interface>(HitActor))
			{
				HeldActor = Interface->Pickup(this);
			}
		}
	}
}

void ATeam_ProjectCharacter::ReleaseActor()
{
		HeldActor->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(HeldActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
		{
			PrimComponent->SetSimulatePhysics(true);
		}
		HeldActor = nullptr;
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
			if (IPickup_Interface* Interface = Cast<IPickup_Interface>(HitActor))
			{
				Interface->Puton();
			}
		}
	}
}

void ATeam_ProjectCharacter::OpenInventory()
{
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
	if ((GetInputAxisValue(TEXT("Move Right / Left")) == 0)&&(GetInputAxisValue(TEXT("Move Forward / Backward")) > 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("We are now sprinting."));
		CurrentStanding = EStanding::ESD_Sprinting;
		SetStanding(CurrentStanding);
	}
	
}

void ATeam_ProjectCharacter::SprintEnd()
{
	if(bSprint == true)
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


void ATeam_ProjectCharacter::MoveForward(float Value)
{
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

void ATeam_ProjectCharacter::MoveRight(float Value)
{

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

void ATeam_ProjectCharacter::DecreaseFoodWater()
{
	CurrentHungry = CurrentHungry - HungryRate;
	CurrentThirsty = CurrentThirsty - ThirstyRate;

	if (CurrentHungry <= 0 || CurrentThirsty <= 0)
	{
		//Destroy();
	}
}
