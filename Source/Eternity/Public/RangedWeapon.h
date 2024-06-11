// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.generated.h"

UCLASS()
class ETERNITY_API ARangedWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta=(AllowPrivateAccess = "true"))
	float MinDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta=(AllowPrivateAccess = "true"))
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage, meta=(AllowPrivateAccess = "true"))
	float MaxDamageReduced;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta=(AllowPrivateAccess = "true"))
	float BaseTimeRanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta=(AllowPrivateAccess = "true"))
	float TimeDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property, meta=(AllowPrivateAccess = "true"))
	float Range;
	
	float CurrentTimeRanged;
	bool bIsHolding;
	
public:	
	// Sets default values for this actor's properties
	ARangedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartShoot();

	void EndShoot();
};
