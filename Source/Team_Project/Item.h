// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_Interface.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class ATeam_ProjectCharacter;

UCLASS(Abstract, BlueprintType, Blueprintable, DefaultToInstanced)
class TEAM_PROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// virtual class UWorld* GetWorld() const { return World; };

	UPROPERTY(Transient)
	class UWorld* World;

	// Pickup Actor and attach to socket
	virtual AActor* Pickup(ATeam_ProjectCharacter* PickingUpActor); // override;

	// hide actor and disable collision
	virtual void Puton(); // override;

	virtual void Drop(); // override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// An optional description fot the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (MultiLine = true))
		FText ItemDescription;

	// The weight of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (ClampMin = 0.0))
		float Weight;

	// The inventory that owns this item
	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	virtual void Use(class ATeam_ProjectCharacter* Character) PURE_VIRTUAL(AItem, );

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class ATeam_ProjectCharacter* Character);


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		USoundCue* EquipSound;


	// The text fot using the item.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		FText UseActionText;

	// The mesh to display fot this items pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		class UstaticMesh* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	// The thumbnail fot this item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		class UTexture2D* Thumbnail;

	// The display name fot tihs item in the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Properties",
		meta = (AllowPrivateAccess = "true"))
		FText ItemDisplayName;

public:
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() { return ItemMesh; }
	FORCEINLINE UStaticMeshComponent* GetMesh() { return Mesh; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
};
