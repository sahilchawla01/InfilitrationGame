// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERASSETPACK_API AShooterCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public: 

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	private:

	UPROPERTY(EditDefaultsOnly)
	float RestartDelay = 5.f;

	FTimerHandle RestartTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* HUD;

};


