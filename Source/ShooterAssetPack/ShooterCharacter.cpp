// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "Public/Gun.h"
#include "Components/CapsuleComponent.h"
#include "KillEmAllEliminationGameMode.h"
 

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialise Health
	Health = MaxHealth;
	
	// Spawn a gun that will be a blueprint, a child of AGun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	// Hide the gun bone to hide the gun in the mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	// Attach Gun to WeaponSocket bone
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());

	if(!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller doesn't exist"));
		return;
	}
	// Get local player subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	// Clear out existing mapping and add own mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	// Get enhancedinputcomponent
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(!PEI)
	{
		UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component couldn't be cast"));
		return;
	}

	// Bind actions
	BindInputActions(PEI);	

}

void AShooterCharacter::Move(const FInputActionValue &Value)
{
	if (!Controller)
    {
		UE_LOG(LogTemp, Warning, TEXT("Character while moving, Controller doesn't exist"));
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();

	// Forward/Backward direction
	if (MoveValue.Y != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), MoveValue.Y * MovementSpeed * GetWorld()->GetDeltaSeconds());
	}

	// Right/Left direction
	if (MoveValue.X != 0.f)
	{
		AddMovementInput(GetActorRightVector(), MoveValue.X * MovementSpeed * GetWorld()->GetDeltaSeconds());
	}

}

void AShooterCharacter::Look(const FInputActionValue &Value)
{
	if (!Controller)
    {
		UE_LOG(LogTemp, Warning, TEXT("Character while looking, Controller doesn't exist"));
		return;
	}

	const FVector2D LookValue = Value.Get<FVector2D>();

	// Deals with pitch
	if(LookValue.Y != 0.f)
	{
		// Negated so that look up down is not inverted
		AddControllerPitchInput(-LookValue.Y * RotationRate * GetWorld()->GetDeltaSeconds());
	}

	// Deals with yaw
	if(LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X * RotationRate * GetWorld()->GetDeltaSeconds());
	}

}

void AShooterCharacter::Shoot(const FInputActionValue &Value)
{
	UE_LOG(LogTemp, Display, TEXT("Shot from Character!"));
	if (!Controller)
    {
		UE_LOG(LogTemp, Warning, TEXT("Character while shooting, Controller doesn't exist"));
		return;
	}

	if(!Gun)
	{
		UE_LOG(LogTemp, Error, TEXT("Gun variable is unassigned"));
		return;
	}

	// Shoot the gun
	Gun->ShootGun();
}


void AShooterCharacter::CustomJump(const FInputActionValue &Value)
{
	UE_LOG(LogTemp, Display, TEXT("Character Jump Pressed"));
	if (!Controller)
    {
		UE_LOG(LogTemp, Warning, TEXT("Character while jumping, Controller doesn't exist"));
		return;
	}

	// Cause to jump
	Jump();
}

/**
 * @brief Binds all inputs and checks if any action input is empty, returns if that is the case
*/
void AShooterCharacter::BindInputActions(UEnhancedInputComponent* PEI)
{
	if(!InputMove)
	{
		UE_LOG(LogTemp, Error, TEXT("UInputAction Input Move is unassigned"));
		return;
	} 
	else 
	{
		PEI->BindAction(InputMove, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);		
	}

	if(!InputLook)
	{
		UE_LOG(LogTemp, Error, TEXT("UInputAction Input Look is unassigned"));
		return;
	} 
	else 
	{
		PEI->BindAction(InputLook, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	}

	if(!InputShoot)
	{
		UE_LOG(LogTemp, Error, TEXT("UInputAction Input Shoot is unassigned"));
		return;
	} 
	else 
	{
		PEI->BindAction(InputShoot, ETriggerEvent::Triggered, this, &AShooterCharacter::Shoot);
	}

	if(!InputJump)
	{
		UE_LOG(LogTemp, Error, TEXT("UInputAction Input Jump is unassigned"));
		return;
	} 
	else 
	{
		PEI->BindAction(InputJump, ETriggerEvent::Triggered, this, &AShooterCharacter::CustomJump);
	}

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DamageToApply = FMath::Min(Health, DamageToApply);
	// Reduce the health
	Health -= DamageToApply;

	if(CheckIfCharacterDead())
	{
		// Get gamemode
		AKillEmAllEliminationGameMode* GameMode = GetWorld()->GetAuthGameMode<AKillEmAllEliminationGameMode>();

		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		// Remove controller
		DetachFromControllerPendingDestroy();
		// Disable collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}

	UE_LOG(LogTemp, Display, TEXT("Current health is %f"), Health);
	return DamageToApply;
}

bool AShooterCharacter::CheckIfCharacterDead() const
{
    return Health <= 0.f ? true : false;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


