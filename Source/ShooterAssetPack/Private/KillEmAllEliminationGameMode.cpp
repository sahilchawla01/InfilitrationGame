// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllEliminationGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllEliminationGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

    APlayerController* PC = Cast<APlayerController>(PawnKilled->GetController());

    // Player has died
    if(PC != nullptr)
    {
        EndGame(false);
    }


    // Game win logic
    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if(!Controller->IsDead()) return;
    }

    // Play has won
    EndGame(true);
}

void AKillEmAllEliminationGameMode::EndGame(bool bIsPlayerWinner)
{
	for(AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
