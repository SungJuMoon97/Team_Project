// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Pickup_Interface.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxItemsInInventory = 5;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::AddItemToInventory(AActor* Item)
{
	if (Items.Num() < MaxItemsInInventory && Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("PICKED UP ACTOR: %s"), *Item->GetName());
		Items.Add(Item);
	}
}
