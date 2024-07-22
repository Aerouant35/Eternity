// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	//ProjectileMovementComponent->bSweepCollision = false;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingAccelerationMagnitude = BulletHomingSpeed;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Init(const AActor* EnemyTarget, const float BulletRange, const float BulletDamage)
{
	if (ProjectileMovementComponent == nullptr) return;
	SetLifeSpan(BulletRange/ProjectileMovementComponent->MaxSpeed);

	BaseDamage = BulletDamage;
	
	if (EnemyTarget == nullptr) return;
	ProjectileMovementComponent->HomingTargetComponent = EnemyTarget->GetRootComponent();
}