// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Public/Gun.h"
#include "ShooterCharacter.generated.h"

class UInputAction;

UCLASS()
class SHOOTERASSETPACK_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool CheckIfCharacterDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UPROPERTY()
	AGun* Gun;

	#pragma region /** Input Action Bound Functions*/
	// Handle move input
	void Move(const FInputActionValue& Value);
	// Handle look input
	void Look(const FInputActionValue& Value);
	// Handle jump input
	void CustomJump(const FInputActionValue& Value);
	// Handle shoot input
	void Shoot(const FInputActionValue& Value);
	#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region /** Input Actions & Mapping Context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* InputMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* InputLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* InputJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    UInputAction* InputShoot;

	
#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

UPROPERTY(EditDefaultsOnly, Category = "Movement Parameters", meta = (AllowPrivateAccess = "true"))
float RotationRate = 1.f;

UPROPERTY(EditDefaultsOnly, Category = "Movement Parameters", meta = (AllowPrivateAccess = "true"))
float MovementSpeed = 1.f;



void BindInputActions(class UEnhancedInputComponent* PlayerInputComp);

UPROPERTY(EditDefaultsOnly)
TSubclassOf<class AGun> GunClass;

UPROPERTY(EditDefaultsOnly, Category = "Pawn Informa")
float MaxHealth = 100.f;

UPROPERTY(VisibleAnywhere)
float Health = 100.f;

};
