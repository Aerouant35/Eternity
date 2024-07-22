// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.generated.h"

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;
};

UINTERFACE(Blueprintable)
class ETERNITY_API UDamageable  : public UInterface
{
	GENERATED_BODY()
	
};

class ETERNITY_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(FHitInfo HitInfo);
};


