// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.generated.h"

UCLASS()
class ETERNITY_API ARangedWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta=(AllowPrivateAccess = "true"))
	float PerfectDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta=(AllowPrivateAccess = "true"))
	float DamageReduced = 8;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta=(AllowPrivateAccess = "true"))
	float MinChargeTime = 0.8f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta=(AllowPrivateAccess = "true"))
	float PerfectChargeTime = 1.f;
	
	UPROPERTY(BlueprintReadOnly, Category = Time, meta=(AllowPrivateAccess = "true"))
	float CurrentChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> BulletClass;
	
	bool bIsAiming;
	// FTimerHandle CurrentTimeRanged;

public:	
	// Sets default values for this actor's properties
	ARangedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Shoot(const AActor* Enemy, const float BulletRange);
	
	void StartShoot();
};
