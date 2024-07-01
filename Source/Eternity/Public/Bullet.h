// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class ETERNITY_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovementComponent;

	float Damage;
	
public:	
	// Sets default values for this actor's properties
	ABullet();
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const AActor* EnemyTarget, const float BulletRange, const float BulletDamage);
};
