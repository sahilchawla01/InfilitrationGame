// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	// Create Skeletal Mesh
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	Mesh->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Shoot the gun
void AGun::ShootGun()
{
	// Play the muzzle flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticle, Mesh, TEXT("MuzzleFlashSocket"));

	// Play muzzle sound
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult TraceResult;
	FVector ShotDirection;

	bool bTraceHit = GunTrace(TraceResult, ShotDirection);
	
	// No hit
	if(!bTraceHit) return;


	// If the particle hit, then spawn particle 
	if(BulletHitParticle)
	{
		// Spawn the particle of hitting
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitParticle, TraceResult.ImpactPoint, ShotDirection.Rotation());
		// Play sound at hit location
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BulletHitSound, TraceResult.ImpactPoint, ShotDirection.Rotation());
	}

	AActor* HitActor = TraceResult.GetActor();

	if(HitActor)
	{
		AController* OwnerController = GetOwnerController(); 
		// Create a damage event
		FPointDamageEvent DamageEvent(Damage, TraceResult, ShotDirection, nullptr);
		// Finally give damage
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

bool AGun::GunTrace(FHitResult& TraceResult, FVector& ShotDirection)
{
	
	AController* OwnerController = GetOwnerController();

	if(!OwnerController) return false;

	FVector CameraLocation;
	FRotator CameraRotation;
	// Get view point parameters
	OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Get end location
	FVector EndLocation = CameraLocation + (CameraRotation.Vector() * MaxRange); 

	// Set shot direction
	ShotDirection = -CameraRotation.Vector();

	// Get the world
	UWorld* ThisWorld = GetWorld();
	if(!ThisWorld) return false;

	// Ignore current gun and owner character
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	// Raycast by line trace
	return ThisWorld->LineTraceSingleByChannel(TraceResult, CameraLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if(!OwnerPawn) return nullptr;

	AController* OwnerController = OwnerPawn->GetController();

	return OwnerController;
}





