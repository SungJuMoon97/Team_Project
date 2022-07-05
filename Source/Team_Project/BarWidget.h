// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_PROJECT_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Update HUD with current health.
	void SetHealth(float CurrentHealth, float MaxHealth);

	// Udate HUD with current Mana
	void SetMana(float CurrentMana, float MaxMana);

	// Widget to use to display current health.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	// Widget to use to display current Mana.
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ManaBar;
};
