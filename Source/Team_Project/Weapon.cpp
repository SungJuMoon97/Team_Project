// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Team_ProjectCharacter.h"
#include "Enum_Collection.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon():
	CurrentWeaponHand(EWeaponHand::EWH_OneHanded),CurrentWeaponType(EWeaponType::EWT_Sword)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->GetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		WP_Sword(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Dagger_2.SK_Dagger_2'"));
	if (WP_Sword.Succeeded())
		GetItemMesh()->SetSkeletalMesh(WP_Sword.Object);

	SetWeaponHand(CurrentWeaponHand);
	SetWeaponType(CurrentWeaponType);
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetWeaponHand(CurrentWeaponHand);
}

void AWeapon::SetWeaponType(EWeaponType Type)
{
	switch (Type)
	{
	case EWeaponType::EWT_Fist:
		
		break;
	case EWeaponType::EWT_Sword:
		
		break;
	case EWeaponType::EWT_Hammer:

		break;
	case EWeaponType::EWT_Spear:

		break;
	case EWeaponType::EWT_Bow:
		
		break;
	}
}

void AWeapon::SetWeaponHand(EWeaponHand Kind)
{
	switch (Kind)
	{
	case EWeaponHand::EWH_OneHanded:
		if (bSwordWeapon)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh>
				WP_Sword(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Dagger_2.SK_Dagger_2'"));
			if (WP_Sword.Succeeded())
				GetItemMesh()->SetSkeletalMesh(WP_Sword.Object);
		}
		break;
		
	case EWeaponHand::EWH_TwoHanded:
		if (bHammerWeapon)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh>
				WP_Hammer(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_GreatHammer.SK_GreatHammer'"));
			if (WP_Hammer.Succeeded())
				GetItemMesh()->SetSkeletalMesh(WP_Hammer.Object);
		}
		if (bSwordWeapon)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh>
				WP_Sword(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'"));
			if (WP_Sword.Succeeded())
				GetItemMesh()->SetSkeletalMesh(WP_Sword.Object);
		}
		break;

	case EWeaponHand::EWH_Fist:
		if (bFistWeapon)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh>
				WP_Knuckle(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_H2H.SK_H2H'"));
			if (WP_Knuckle.Succeeded())
				GetItemMesh()->SetSkeletalMesh(WP_Knuckle.Object);
		}
		break;

	case EWeaponHand::EWH_Bow:
		if (bBowWeapon)
		{
			static ConstructorHelpers::FObjectFinder<USkeletalMesh>
				WP_Bow(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Bow.Bow'"));
			if (WP_Bow.Succeeded())
				GetItemMesh()->SetSkeletalMesh(WP_Bow.Object);
		}
		break;
	}
}

void AWeapon::WeaponCheck()
{

}


AActor* AWeapon::Pickup(ATeam_ProjectCharacter* PickingUpActor)
{
	const USkeletalMeshSocket* UnEquipSocket = MyCharacter->GetMesh()->GetSocketByName(FName("UnEquip_SWeaponSocket"));
	//AttachToComponent(PickingUpActor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_Socket"));
	UnEquipSocket->AttachActor(this, MyCharacter->GetMesh());
	return this;
}

void AWeapon::SetItemState(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_Ground:
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetEnableGravity(true);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::EIS_Equip:
		// Set mesh properties
		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Puton:
		// hide actor and disable collision
		UE_LOG(LogTemp, Warning, TEXT("Hide!"));
		ItemMesh->SetHiddenInGame(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Drop:
		// drop actor and enable collision
		ItemMesh->SetHiddenInGame(false);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

/*

*/