// Copyright Epic Games, Inc. All Rights Reserved.

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

ATeam_ProjectCharacter::ATeam_ProjectCharacter():
	CurrentViewMode(EViewType::EVT_FirstPerson), CurrentStanceMode(EStance::ES_Default),CurrentWeapon(EWeaponType::EWT_Fist),
	BareHandDamage(10),bLeftHandAction(false),bRightHandAction(false),CurrentHandWeapon(EWeaponHand::EWH_Fist)
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

	FName FirstPersonCameraSocket(TEXT("FirstPersonCameraSocket"));
	FName ThirdPersonCameraSocket(TEXT("ThirdPersonCameraSocket"));
	FirstPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("FirstPersonCameraBoom"));
	FirstPersonFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonFollowCamera"));
	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonCameraBoom"));
	ThirdPersonFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonFollowCamera"));

	ThirdPersonCameraBoom->TargetArmLength = 150.f;
	ThirdPersonCameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);
	ThirdPersonFollowCamera->SetupAttachment(ThirdPersonCameraBoom, USpringArmComponent::SocketName);

	//FirstPersonFollowCamera->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform,FirstPersonCameraSocket);
	FirstPersonFollowCamera->SetupAttachment(GetMesh(), FirstPersonCameraSocket);
	FirstPersonFollowCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonFollowCamera->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	bUseControllerRotationYaw = true;
	FirstPersonFollowCamera->bUsePawnControlRotation = true;
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	//SetViewType(CurrentViewMode);
	
}

void ATeam_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

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
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
		return;
	}


	if (!this->GetCharacterMovement()->IsFalling())
	{
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{

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
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
		return;
	}

	if (!this->GetCharacterMovement()->IsFalling())
	{
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{

		}
		else
		{
			if (AnimInstance && RightPunchingMontage && (CurrentWeapon == EWeaponType::EWT_Fist))
			{
				//PlayAnimMontage(LeftPunchingMontage, 1, NAME_None);


				AnimInstance->Montage_Play(RightPunchingMontage);
				bDoAttacking = true;
				UE_LOG(LogTemp, Warning, TEXT("Punch"));
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("EndARightHand"));
	bDoAttacking = false;
}

void ATeam_ProjectCharacter::SetViewType(EViewType ViewType)
{
	
	switch (ViewType)
	{
	case EViewType::EVT_ThirdPerson:
		FirstPersonFollowCamera->Deactivate();
		ThirdPersonFollowCamera->Activate();

		break;

	case EViewType::EVT_FirstPerson:
		FirstPersonFollowCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
		FirstPersonFollowCamera->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
		ThirdPersonFollowCamera->Deactivate();
		FirstPersonFollowCamera->Activate();
		
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
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
			
		UE_LOG(LogTemp, Warning, TEXT("StanceChanged"));
	}
	else
	{
		CurrentStanceMode = EStance::ES_Default;
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		SetStanceType(CurrentStanceMode);
	}
}

void ATeam_ProjectCharacter::SetStanceType(EStance StanceType)
{
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	switch (StanceType)
	{
	case EStance::ES_Default:
		bCombatState = false;
		
		//MKKS_Controller->bShowMouseCursor = false;
		break;
	case EStance::ES_Combat:
		bCombatState = true;
		//MKKS_Controller->bShowMouseCursor = true;
		break;
	}
}


void ATeam_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LeftHand", IE_Pressed, this, &ATeam_ProjectCharacter::LeftHand);
	PlayerInputComponent->BindAction("RightHand", IE_Pressed, this, &ATeam_ProjectCharacter::RightHand);;
	PlayerInputComponent->BindAction("ViewChange", IE_Pressed, this, &ATeam_ProjectCharacter::ViewChange);
	PlayerInputComponent->BindAction("StanceChange", IE_Pressed, this, &ATeam_ProjectCharacter::StanceChange);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATeam_ProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATeam_ProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATeam_ProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATeam_ProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATeam_ProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATeam_ProjectCharacter::TouchStopped);
}

void ATeam_ProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATeam_ProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
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
	//if (Value != 0.0f)
	//{
	//	// add movement in that direction
	//	AddMovementInput(GetActorForwardVector(), Value);
	//}
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

	//if (Value != 0.0f)
	//{
	//	// add movement in that direction
	//	AddMovementInput(GetActorRightVector(), Value);
	//}

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
