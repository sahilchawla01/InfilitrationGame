// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTERASSETPACK_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void ShootGun();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* Mesh; 

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* MuzzleFlashParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* BulletHitParticle;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float MaxRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* MuzzleSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BulletHitSound;

	bool GunTrace(FHitResult& TraceResult, FVector& ShotDirection);

	AController* GetOwnerController() const; 

};


