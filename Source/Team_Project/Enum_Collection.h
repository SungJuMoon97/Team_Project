#pragma once

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	EWT_Fist UMETA(DisplayName = "Fist"),
	EWT_Sword UMETA(DisplayName = "Sword"),
	EWT_Hammer UMETA(DisplayName = "Hammer"),
	EWT_Spear UMETA(DisplayName = "Spear"),
	EWT_Pole UMETA(DisplayName = "Pole"),
	EWT_Bow UMETA(DisplayName = "Bow"),

	EWT_MAX UMETA(DisplayName = "EWT_MAX")
};