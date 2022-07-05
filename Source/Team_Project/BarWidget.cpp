// Fill out your copyright notice in the Description page of Project Settings.


#include "BarWidget.h"
#include "Components/ProgressBar.h"

void UBarWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UBarWidget::SetMana(float CurrentMana, float MaxMana)
{
	ManaBar->SetPercent(CurrentMana / MaxMana);
}
