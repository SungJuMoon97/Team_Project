// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "InventoryComponent.h"
#include "Enum_Collection.h"
#include "Team_ProjectCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AItem::AItem():
	ItemState(EItemState::EIS_Ground)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InsItemMesh"));
	SetItemState(ItemState);
}

AActor* AItem::Pickup(ATeam_ProjectCharacter* PickingUpActor)
{
	if (GetRootComponent()->IsSimulatingPhysics())
	{
		if (UPrimitiveComponent* PrimComponent = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass())))
		{
			PrimComponent->SetSimulatePhysics(false);
			//SetItemState(EItemState::EIS_Equip);
			AttachToComponent(PickingUpActor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			return this;
		}
	}
	else
	{
		//SetItemState(EItemState::EIS_Equip);
		AttachToComponent(PickingUpActor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
		return this;
	}
	return nullptr;
}

void AItem::SetItemState(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_Ground:
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetVisibility(true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EItemState::EIS_Equip:
		// Set mesh properties
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetVisibility(true);
		//Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Puton:
		// hide actor and disable collision
		UE_LOG(LogTemp, Warning, TEXT("Hide!"));
		Mesh->SetHiddenInGame(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EItemState::EIS_Drop:
		// drop actor and enable collision
		Mesh->SetHiddenInGame(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemState(ItemState);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

