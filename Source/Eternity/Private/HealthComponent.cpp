// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ComponentUtils.h"
#include "EternityCharacter.h"
#include "TranscendentFormComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::Init()
{
	CurrentHealth = MaxHealth;
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, (GetOwner()->GetClass()->GetName()));

}

void UHealthComponent::TakeDamage(const FHitInfo HitInfo)
{
	IDamageable::TakeDamage(HitInfo);

	AEternityCharacter* EternityCharacter = Cast<AEternityCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (EternityCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("TakeDamage event"));
		EternityCharacter->TranscendentFormComponent->OnIncreaseGauge.Execute(GetOwner(), HitInfo.Damage);
	}
	
	CurrentHealth -= HitInfo.Damage;
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%d"), CurrentHealth));
	if (CurrentHealth <= 0) Death();
}

void UHealthComponent::Death()
{
	GetOwner()->Destroy();
}
