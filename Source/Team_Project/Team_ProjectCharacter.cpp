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

ATeam_ProjectCharacter::ATeam_ProjectCharacter():
	CurrentViewMode(EViewType::EVT_ThirdPerson), CurrentStanceMode(EStance::ES_Default)
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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	CameraBoom->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform);
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	FollowCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	
	CameraBoom->bUsePawnControlRotation = true;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	ViewChange();
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
		bUseControllerRotationYaw = true;
		CameraBoom->bUsePawnControlRotation = true;
		CameraBoom->TargetArmLength = 150.f;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
		CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		FollowCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetIncludingScale,USpringArmComponent::SocketName);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case EViewType::EVT_FirstPerson:
		
		bUseControllerRotationYaw = true;
		FollowCamera->bUsePawnControlRotation = true;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, -70.0f));
		FollowCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		FollowCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,FirstPersonCameraSocket);
		FollowCamera->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
		GetCharacterMovement()->bOrientRotationToMovement = false;
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
	//AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
	if (Rate)
	{
		float temp = 0;
		if (CurrentViewMode == EViewType::EVT_FirstPerson)
		{
			temp = FollowCamera->GetRelativeRotation().Pitch + Rate;
			if(temp < 65 && temp >-65)
				FollowCamera->AddLocalRotation(FRotator(Rate, 0, 0));
				//AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			temp = CameraBoom->GetRelativeRotation().Pitch + Rate;
			if(temp < 25 && temp > - 65)
			{
				CameraBoom->AddLocalRotation(FRotator(Rate, 0, 0));
			}
		}
	}
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
