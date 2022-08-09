#include "InstanceItem.h"
#include "Components/StaticMeshComponent.h"
#include "Team_ProjectCharacter.h"

// Sets default values
AInstanceItem::AInstanceItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InsItemMesh"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AInstanceItem::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* AInstanceItem::Pickup(ATeam_ProjectCharacter* PickingUpActor)
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

<<<<<<< HEAD
//void AInstanceItem::Puton()
//{
//	// hide actor and disable collision
//	Mesh->SetHiddenInGame(true);
//	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//}
//
//void AInstanceItem::Drop()
//{
//	// drop actor and enable collision
//	Mesh->SetHiddenInGame(false);
//	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//}
=======
void AInstanceItem::Puton()
{
	// hide actor and disable collision
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInstanceItem::Drop()
{
	// drop actor and enable collision
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
>>>>>>> origin/KPrevious-Work
