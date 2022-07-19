// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Pickup_Interface.h"
#include "Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// MaxItemsInInventory = 2;
	Capacity = 20;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}

}


/*
void UInventoryComponent::AddItemToInventory(AActor* Item)
{
	if (Items.Num() < MaxItemsInInventory && Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("PICKED UP ACTOR: %s"), *Item->GetName());
		Items.Add(Item);
	}
}
*/

/*
void UInventoryComponent::DropItem(AActor* Item)
{
	// test later if returned find is valid for if statement
	if (int FoundIndex = Items.Find(Item))
	{
		if (IPickup_Interface* Interface = Cast<IPickup_Interface>(Item))
		{
			// Set actor location somewhere infront of us
			UE_LOG(LogTemp, Warning, TEXT("DROPPING ACTOR %s"), *Item->GetName());
			Interface->Drop();
			Items.RemoveAt(FoundIndex);
		}
	}
}
*/

bool UInventoryComponent::AddItem(AItem* Item)
{
	if (Items.Num() >= Capacity || !Item)
	{
		return false;
	}

	Item->OwningInventory = this;
	Item-> World = GetWorld();
	Items.Add(Item);
	return false;
	// Update UI
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(AItem* Item)
{
	if (Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();

		return true;
	}
	return false;
}

