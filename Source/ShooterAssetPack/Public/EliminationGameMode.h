// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EliminationGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERASSETPACK_API AEliminationGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:

	virtual void PawnKilled(APawn* PawnKilled);
	
};
