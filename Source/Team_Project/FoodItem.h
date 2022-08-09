// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_PROJECT_API AFoodItem : public AItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (ClampMin = 0.0))
		float HealthToHeal;


protected:
	virtual void Use(class ATeam_ProjectCharacter* Character) override;

};
