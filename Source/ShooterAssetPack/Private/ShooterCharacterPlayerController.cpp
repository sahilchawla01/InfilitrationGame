// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Controller.h"

void AShooterCharacterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add HUD
    HUD = CreateWidget(this, HUDClass);
    HUD->AddToViewport();
}


void AShooterCharacterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

    if(bIsWinner)
    {
        // Show Win screen UI
        // Create the widget
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

        if(WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }

    }
    else 
    {
        // Show Lose screen UI
        // Create the widget
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

        if(LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }

    // Remove HUD UI
    HUD->RemoveFromViewport();

    // Restart Level after 5 seconds
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}


