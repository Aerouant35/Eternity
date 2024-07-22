// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TranscendentFormComponent.generated.h"

class AEternityCharacter;
class ABullet;
class UTranscendentAsset;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnIncreaseGauge, AActor*, Victim, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNITY_API UTranscendentFormComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AEternityCharacter> EternityCharacter;

#pragma region TranscendentParameter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UTranscendentAsset> TranscendentAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	float DamageDealtTranscendentMultiplier = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	float DamageTakenTranscendentMultiplier = 60.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	float TranscendentGaugeLimit = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	float CurrentTranscendentGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter, meta=(AllowPrivateAccess = "true"))
	float TranscendentGaugeDuration = 60.f;
#pragma endregion

#pragma region Ranged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RangedTime, meta=(AllowPrivateAccess = "true"))
	float MinChargeTime = 0.8f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RangedTime, meta=(AllowPrivateAccess = "true"))
	float PerfectChargeTime = 1.f;
	
	UPROPERTY(BlueprintReadOnly, Category = RangedTime, meta=(AllowPrivateAccess = "true"))
	float CurrentChargeTime;
#pragma endregion

#pragma region Finisher
	
#pragma endregion 
	
	bool bIsAiming;
	FTimerHandle TranscendentTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter)
	bool bIsTranscendentFormActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TranscendentParameter)
	bool bIsFinisherActive = false;
	
	UPROPERTY(BlueprintReadOnly)
	FOnIncreaseGauge OnIncreaseGauge;
	
	UTranscendentFormComponent();

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(AEternityCharacter* Ec);

	UFUNCTION()
	void IncreaseGauge(AActor* Victim, float Damage);
	void ActivateTranscendentForm();

	UFUNCTION(BlueprintCallable)
	void DeactivateTranscendentForm();

	void Melee();
	void StartShoot();

	UFUNCTION(BlueprintCallable)
	void Shoot(const AActor* Enemy, const float BulletRange);
	void Finisher();
};
