// Fill out your copyright notice in the Description page of Project Settings.


#include "TranscendentFinisher.h"

class UTranscendentFormComponent;

ATranscendentFinisher::ATranscendentFinisher()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATranscendentFinisher::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATranscendentFinisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATranscendentFinisher::LaunchFinisher_Implementation(UTranscendentFormComponent* TranscendentFormComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Maybe"));
}
