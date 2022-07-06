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

ATeam_ProjectCharacter::ATeam_ProjectCharacter()//:CurrentViewMode(EViewType::EVT_FirstPerson)
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		Shinbi_Mesh(TEXT("SkeletalMesh'/Game/Retarget/Meshes/Shinbi_NoWeapon.Shinbi_NoWeapon'"));
	if (Shinbi_Mesh.Succeeded())
		GetMesh()->SetSkeletalMesh(Shinbi_Mesh.Object);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	
	SetViewType(EViewType::EVT_ThirdPerson);
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

void ATeam_ProjectCharacter::DoAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"));
}

void ATeam_ProjectCharacter::EndAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
}

void ATeam_ProjectCharacter::SetViewType(EViewType ViewType)
{
	FName FirstPersonCameraSocket(TEXT("FirstPersonCameraSocket"));
	FName ThirdPersonCameraSocket(TEXT("ThirdPersonCameraSocket"));

	switch (ViewType)
	{
	case EViewType::EVT_ThirdPerson:

		CameraBoom->SetupAttachment(RootComponent,ThirdPersonCameraSocket);
		//CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ThirdPersonCameraSocket);
		//CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character
		CameraBoom->AddRelativeLocation(FVector(0.0f,30.0f,0.0f));
		CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
		CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
		break;

	case EViewType::EVT_FirstPerson:

		//CameraBoom->SetupAttachment(RootComponent, FirstPersonCameraSocket);
		CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FirstPersonCameraSocket);
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
		CameraBoom->TargetArmLength = 30.0f;  // The camera follows at this distance behind the character
		CameraBoom->AddRelativeLocation(FVector(0.0f,20.0f,0.0f));
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
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

void ATeam_ProjectCharacter::SetDefaultStance()
{

}

void ATeam_ProjectCharacter::SetCombatStance()
{

}

void ATeam_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ATeam_ProjectCharacter::DoAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ATeam_ProjectCharacter::EndAttack);
	PlayerInputComponent->BindAction("ViewChange", IE_Pressed, this, &ATeam_ProjectCharacter::ViewChange);
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

void ATeam_ProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
