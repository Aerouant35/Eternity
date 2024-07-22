// Fill out your copyright notice in the Description page of Project Settings.


#include "TranscendentFormComponent.h"

#include "Bullet.h"
#include "EternityCharacter.h"
#include "HealthComponent.h"
#include "TranscendentAsset.h"
#include "TranscendentFinisher.h"
#include "Kismet/KismetSystemLibrary.h"

UTranscendentFormComponent::UTranscendentFormComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTranscendentFormComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnIncreaseGauge.BindDynamic(this, &UTranscendentFormComponent::IncreaseGauge);
}

void UTranscendentFormComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTranscendentFormComponent::Init(AEternityCharacter* Ec)
{
	EternityCharacter = Ec;
}

void UTranscendentFormComponent::IncreaseGauge(AActor* Victim, const float Damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Silver, TEXT("IncreaseGauge"));

	if (CurrentTranscendentGauge >= TranscendentGaugeLimit) return;

	if (Cast<AEternityCharacter>(Victim))
	{
		CurrentTranscendentGauge += Damage / EternityCharacter->HealthComponent->CurrentHealth * DamageTakenTranscendentMultiplier;
	}
	
	if (Victim->IsA(EternityCharacter->EnemyClass))
	{
		CurrentTranscendentGauge += Damage * DamageDealtTranscendentMultiplier;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Silver, FString::Printf(TEXT("TranscendentGauge current %f"), CurrentTranscendentGauge));
}

void UTranscendentFormComponent::Melee()
{
}

void UTranscendentFormComponent::StartShoot()
{
	if (bIsAiming) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("StartTranscendenceShoot"));

	CurrentChargeTime = 0;
	bIsAiming = true;
}

void UTranscendentFormComponent::Shoot(const AActor* Enemy, const float BulletRange)
{
	bIsAiming = false;
	
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(TranscendentAsset->BulletClass, EternityCharacter->GetWeaponSocketLocation(), EternityCharacter->GetActorRotation());
	Bullet->Init(Enemy, BulletRange, 10);
	
	if (Bullet) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Acteur spawn"));
	else GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Acteur pas spawn"));
}

void UTranscendentFormComponent::Finisher()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Finisher"));

	bIsFinisherActive = true;
	
	ATranscendentFinisher* TranscendentFinisher = GetWorld()->SpawnActor<ATranscendentFinisher>(TranscendentAsset->TranscendentFinisherClass);
	TranscendentFinisher->LaunchFinisher(this);
}

void UTranscendentFormComponent::ActivateTranscendentForm()
{
	//if (CurrentTranscendentBar < TranscendentBarThreshold) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("ActivateTranscendentForm"));

	GetWorld()->GetTimerManager().SetTimer(TranscendentTimerHandle, [this] { UTranscendentFormComponent::DeactivateTranscendentForm(); }, TranscendentGaugeDuration, false);
	bIsTranscendentFormActive = true;

	EternityCharacter->SetWeaponsActive(false);
}

void UTranscendentFormComponent::DeactivateTranscendentForm()
{
	if (bIsFinisherActive) return;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("DeactivateTranscendentForm"));

	GetWorld()->GetTimerManager().ClearTimer(TranscendentTimerHandle);
	bIsTranscendentFormActive = false;
	
	EternityCharacter->SetWeaponsActive(true);
}