// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "Team_ProjectCharacter.h"

void AFoodItem::Use(class ATeam_ProjectCharacter* Character)
{
	if (Character)
	{
		Character->CurrentHealth += HealthToHeal;
	}
}
