// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//Get ProjectileMovement component
	TArray<UProjectileMovementComponent*> ProjectileComponent;
	GetComponents<UProjectileMovementComponent>(ProjectileComponent);
	ProjectileMovementComponent = ProjectileComponent[0];
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Init(const AActor* EnemyTarget, const float BulletRange, const float BulletDamage)
{
	if (ProjectileMovementComponent == nullptr) return;
	SetLifeSpan(BulletRange/ProjectileMovementComponent->MaxSpeed);

	if (EnemyTarget == nullptr) return;
	ProjectileMovementComponent->HomingTargetComponent = EnemyTarget->GetRootComponent();
}