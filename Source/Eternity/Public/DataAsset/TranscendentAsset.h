// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TranscendentAsset.generated.h"

class ATranscendentFinisher;
class ABullet;
class UNiagaraSystem;

UCLASS(BlueprintType)
class ETERNITY_API UTranscendentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageMultiplier;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> TranscendentForm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
	TSubclassOf<ATranscendentFinisher> TranscendentFinisherClass;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TObjectPtr<UNiagaraSystem> ParticleBullet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> ParticleOnHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound;
};
