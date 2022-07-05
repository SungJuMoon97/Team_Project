#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Fist UMETA(DisplayName = "Fist"),
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Hammer UMETA(DisplayName = "Hammer"),
	EWT_Spear UMETA(DisplayName = "Spear"),
	EWT_Pole UMETA(DisplayName = "Pole"),
	EWT_Bow UMETA(DisplayName = "Bow"),

	EWT_MAX UMETA(DisplayName = "EWT_MAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Armor UMETA(DisplayName = "Armor"),
	EIT_Consumables UMETA(DisplayName = "Consumables"),//소모품
	EIT_Material UMETA(DisplayName = "Material"),//재료

	EIT_MAX UMETA(DisplayName = "EIT_MAX")
};

