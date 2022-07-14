// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DealDamageNotify.generated.h"

/**
 * 
 */
UCLASS()
class TEAM_PROJECT_API UDealDamageNotify : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:


	UFUNCTION(BlueprintCallable)
		void HandCheck(ATeam_ProjectCharacter* Character);

private:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Character,
		meta = (AllowPrivateAccess = "true"))
		class ATeam_ProjectCharacter* Team_ProjectCharacter;

};
