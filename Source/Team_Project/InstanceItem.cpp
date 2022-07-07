#include "InstanceItem.h"
#include "Components/StaticMeshComponent.h"

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


