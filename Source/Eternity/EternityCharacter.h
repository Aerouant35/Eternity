// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include <limits>
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Public/MeleeWeapon.h"
#include "Public/RangedWeapon.h"
#include "EternityCharacter.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UEnhancedInputComponent;
class UTranscendentFormComponent;
class UHealthComponent;
class ARangedWeapon;
class AMeleeWeapon;

UCLASS(config=Game)
class AEternityCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoomComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;

#pragma region Input
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Dodge Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	/** Melee Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MeleeAction;

	/** Ranged Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RangedAction;

	/** Power Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PowerAction;

	/** AimMouse Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimMouseAction;

	/** AimGamepad Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimGamepadAction;
#pragma endregion 

#pragma region PlayerParam
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float DeltaDodge = 8000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float DodgeCooldown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	bool bShouldMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	bool bIsShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	TArray<AActor*> ChildActors;
#pragma endregion 

#pragma region AimAssistParam
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	float DeltaStartBox = 70.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	float DeltaEndBox = 1000.f; // Need to be the end of the range of the ranged weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	FVector BoxSize = FVector(0, 100, 100);
#pragma endregion
	
	FTimerHandle DodgeTimerHandle;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UTranscendentFormComponent> TranscendentFormComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	FVector WeaponSocketLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<AMeleeWeapon> MeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<ARangedWeapon> RangedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(BlueprintReadOnly, Category = Parameter)
	TObjectPtr<AActor> ClosestEnemy = nullptr;

	AEternityCharacter();

	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintCallable)
	void EnableEnhancedInput(const bool Enable) const;
	
	UFUNCTION(BlueprintCallable)
	FVector GetWeaponSocketLocation() const	{ return GetMesh()->GetSocketLocation("Weapon"); }

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void AimMouse(const FInputActionValue& Value);
	void AimGamepad(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void Melee(const FInputActionValue& Value);
	void RangedStart(const FInputActionValue& Value);
	void RangedEnd(const FInputActionValue& Value);
	void Power(const FInputActionValue& Value);
	
	void AimAssist();
	
	FVector2D GetMousePosition() const;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoomComponent; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComponent; }

	UFUNCTION(BlueprintCallable)
	void SetWeaponsActive(bool Active);
};
