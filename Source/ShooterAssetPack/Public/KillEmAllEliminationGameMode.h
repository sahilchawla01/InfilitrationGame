// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EliminationGameMode.h"
#include "KillEmAllEliminationGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERASSETPACK_API AKillEmAllEliminationGameMode : public AEliminationGameMode
{
	GENERATED_BODY()

	public:

	virtual void PawnKilled(APawn* PawnKilled) override;

	private:

	void EndGame(bool bIsPlayerWinner);
	
};


