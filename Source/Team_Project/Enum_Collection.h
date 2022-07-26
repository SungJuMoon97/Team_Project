#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Fist UMETA(DisplayName = "Fist"),
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Hammer UMETA(DisplayName = "Hammer"),
	EWT_Spear UMETA(DisplayName = "Spear"),
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

UENUM(BlueprintType)
enum class  EStance : uint8 //기본자세와 전투자세
{
	ES_Default UMETA(DisplayName = "Default_Stance"),
	ES_Combat UMETA(DisplayName = "Combat_Stance"),

	ES_MAX UMETA(DisplayName = "ES_MAX")
};

UENUM(BlueprintType)
enum class EViewType : uint8
{
	EVT_ThirdPerson UMETA(DisplayName = "ThirdPerson_View"),
	EVT_FirstPerson UMETA(DisplayName = "FirstPerson_View"),


	EVT_MAX UMETA(DisplayName = "EVT_MAX")
};

UENUM(BlueprintType)
enum class EWeaponHand : uint8
{
	EWH_OneHanded UMETA(DisplayName = "OneHandedWeapon"),
	EWH_TwoHanded UMETA(DisplayName = "TwoHandedWeapon"),
	EWH_Fist UMETA(DisplayName = "FistWeapon"),
	EWH_Bow UMETA(DisplayName = "BowWeapon"),

	EWH_MAX UMETA(DisplayName = "EWH_MAX")
};

UENUM(BlueprintType)
enum class EStanding : uint8
{
	ESD_Standing UMETA(DisplayName = "Standing"),
	ESD_Sitting UMETA(DisplayName = "Sitting"),
	ESD_Crouching UMETA(DisplayName = "Crouching"),
	ESD_LayingDown UMETA(DisplayName = "LyingDown"),
	ESD_Sprinting UMETA(DisplayName = "Sprinting"),

	ESD_MAX UMETA(DisplayName = "ESD_MAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Ground UMETA(DisplayName = "Ground"),
	EIS_Equip UMETA(DisplayName = "Equip"),
	EIS_Puton UMETA(DisplayName = "Puton"),
	EIS_Drop UMETA(DisplayName = "Drop"),

	EIS_MAX UMETA(DisplayName = "EIS_MAX")
};