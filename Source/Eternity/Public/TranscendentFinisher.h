// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TranscendentFinisher.generated.h"

class UTranscendentFormComponent;

UCLASS()
class ETERNITY_API ATranscendentFinisher : public AActor
{
	GENERATED_BODY()
	
public:	
	ATranscendentFinisher();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void LaunchFinisher(UTranscendentFormComponent* TranscendentFormComponent);
};
