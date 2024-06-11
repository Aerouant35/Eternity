// Fill out your copyright notice in the Description page of Project Settings.


#include "Eternity/Public/RangedWeapon.h"

// Sets default values
ARangedWeapon::ARangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinDamage = 0;
	MaxDamage = 0;
	MaxDamageReduced = 0;
	BaseTimeRanged = 0;
	TimeDelta = 0;
	CurrentTimeRanged = 0;

	bIsHolding = false;
}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHolding)
		CurrentTimeRanged += DeltaTime;
}

void ARangedWeapon::StartShoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("RangedStart"));

	CurrentTimeRanged = 0;
	bIsHolding = true;
}

void ARangedWeapon::EndShoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("RangedEnd"));

	bIsHolding = false;

	if (CurrentTimeRanged > CurrentTimeRanged - TimeDelta && CurrentTimeRanged < CurrentTimeRanged + TimeDelta)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Perfect ranged time"));
	}
}