// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enum_Collection.h"

AWeapon::AWeapon() :AttackRange(45.0f),AttackDamageMin(10.0f),AttackDamageMax(10.0f)
{

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetWeaponType(EWeaponType Value)
{
	switch (Value)
	{
	case EWeaponType::EWT_Fist:

		break;
	case EWeaponType::EWT_Sword:

		break;
	case EWeaponType::EWT_Hammer:

		break;
	case EWeaponType::EWT_Spear:

		break;
	case EWeaponType::EWT_Pole:

		break;
	case EWeaponType::EWT_Bow:

		break;
	}
}
