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


UCLASS(config=Game)
class AEternityCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

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
	bool bIsShoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	TArray<AActor*> ChildActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<AActor> EnemyClass;
#pragma endregion 

#pragma region AimAssistParam
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	float DeltaStartBox = 70.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	float DeltaEndBox = 1000.f; // Need to be the end of the range of the ranged weapon
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AimAssistParameter, meta=(AllowPrivateAccess = "true"))
	FVector BoxSize = FVector(0, 100, 100);
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta=(AllowPrivateAccess = "true"))
	AMeleeWeapon* MeleeWeapon;

	UPROPERTY(BlueprintReadOnly, Category = Weapon, meta=(AllowPrivateAccess = "true"))
	ARangedWeapon* RangedWeapon;

	UPROPERTY()
	APlayerController* PlayerController;
	
	FTimerHandle DodgeTimerHandle;

public:
	AEternityCharacter();

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	AActor* ClosestEnemy = nullptr;

protected:
	void AimAssist();
	
	void AimMouse(const FInputActionValue& Value);
	
	void AimGamepad(const FInputActionValue& Value);
	
	void Move(const FInputActionValue& Value);
	
	void Dodge(const FInputActionValue& Value);

	void Melee(const FInputActionValue& Value);

	void RangedStart(const FInputActionValue& Value);
	
	void RangedEnd(const FInputActionValue& Value);

	void Power(const FInputActionValue& Value);

	FVector2D GetMousePosition() const;
	
protected:
	virtual void PossessedBy(AController* NewController) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }
};
