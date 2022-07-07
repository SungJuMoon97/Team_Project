#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickup_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickup_Interface : public UInterface
{
	GENERATED_BODY()
};

class ATeam_ProjectCharacter;

class TEAM_PROJECT_API IPickup_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual AActor* Pickup(ATeam_ProjectCharacter* PickingUpActor) { return nullptr; }
	virtual void Puton() = 0;
	virtual void Drop() = 0;
	
};
