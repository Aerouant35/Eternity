// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AEternityCharacter;
class APlayerController;
class UTranscendentFormComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNITY_API UHealthComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AEternityCharacter> EternityCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	int MaxHealth;

public:
	int CurrentHealth;

	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(const FHitInfo HitInfo) override;
	
	void Death();
};
