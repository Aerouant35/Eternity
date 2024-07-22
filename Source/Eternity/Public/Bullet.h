// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class ETERNITY_API ABullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float BulletSpeed = 3000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float BulletHomingSpeed = 10000;

public:	
	ABullet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parameter)
	float BaseDamage;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Init(const AActor* EnemyTarget, const float BulletRange, const float BulletDamage);
};
