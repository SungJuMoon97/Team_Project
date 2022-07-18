// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "InventoryComponent.h"
#include "Team_ProjectCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InsItemMesh"));
	

}

AActor* AItem::Pickup(ATeam_ProjectCharacter* PickingUpActor)
{
	if (GetRootComponent()->IsSimulatingPhysics())
	{
		if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass())))
		{
			PrimComponent->SetSimulatePhysics(false);
			AttachToComponent(PickingUpActor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			return this;
		}
	}
	else
	{
		AttachToComponent(PickingUpActor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
		return this;
	}
	return nullptr;
}

void AItem::Puton()
{
	// hide actor and disable collision
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::Drop()
{
	// drop actor and enable collision
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

