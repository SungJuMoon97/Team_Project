// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_Interface.h"
#include "InstanceItem.generated.h"

class UStaticMeshComponent;
class ATeam_ProjectCharacter;

UCLASS()
class TEAM_PROJECT_API AInstanceItem : public AActor, public IPickup_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInstanceItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "InsItem")
	UStaticMeshComponent* Mesh;

public:
	// Pickup Actor and attach to socket
	virtual AActor* Pickup(ATeam_ProjectCharacter* PickingUpActor) override;

	// hide actor and disable collision
	/*virtual void Puton() override;

	virtual void Drop() override;*/
};
