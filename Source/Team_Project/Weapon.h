// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Enum_Collection.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_PROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	virtual void Tick(float DeltaTime);
	void GetWeaponType(EWeaponType Value);

protected:

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack,
			meta = (AllowPrivateAccess = "true"))
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack,
		meta = (AllowPrivateAccess = "true"))
		float AttackDamageMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack,
		meta = (AllowPrivateAccess = "true"))
		float AttackDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack,
		meta = (AllowPrivateAccess = "true"))
		float AttackModifierMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack,
		meta = (AllowPrivateAccess = "true"))
		float AttackModifierMax;



public:



};
