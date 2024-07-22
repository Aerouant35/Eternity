// Fill out your copyright notice in the Description page of Project Settings.


#include "Eternity/Public/RangedWeapon.h"

// Sets default values
ARangedWeapon::ARangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	bIsAiming = false;
}

void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAiming)
	{
		CurrentChargeTime += DeltaTime;

		if (CurrentChargeTime >= MinChargeTime && CurrentChargeTime <= PerfectChargeTime)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Perfect charge time"));
		}
	}
}

void ARangedWeapon::SetActive(const bool Active)
{
	SetActorTickEnabled(Active);
	SetActorEnableCollision(Active);
	SetActorHiddenInGame(!Active);
}

void ARangedWeapon::Shoot(const AActor* Enemy, const float BulletRange)
{
	bIsAiming = false;
	
	const USceneComponent* Muzzle = FindComponentByClass<USceneComponent>();
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, Muzzle->GetComponentTransform());

	if (CurrentChargeTime >= MinChargeTime && CurrentChargeTime <= PerfectChargeTime)
		Bullet->Init(Enemy, BulletRange, PerfectDamage);
	else
		Bullet->Init(Enemy, BulletRange, DamageReduced);

	if (Bullet) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Acteur spawn"));
	else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Acteur pas spawn"));
}

void ARangedWeapon::StartShoot()
{
	if (bIsAiming) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("RangedStart"));

	CurrentChargeTime = 0;
	bIsAiming = true;
}