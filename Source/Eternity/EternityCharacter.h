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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta=(AllowPrivateAccess = "true"))
	AMeleeWeapon* MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta=(AllowPrivateAccess = "true"))
	ARangedWeapon* RangedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float DeltaDodge = 8000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	float DodgeCooldown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	bool bIsShoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter, meta=(AllowPrivateAccess = "true"))
	bool bShouldMove;
	
	FTimerHandle DodgeTimerHandle;

public:
	AEternityCharacter();

protected:
	void Move(const FInputActionValue& Value);
	
	void Dodge(const FInputActionValue& Value);

	void Melee(const FInputActionValue& Value);

	void RangedStart(const FInputActionValue& Value);
	void RangedEnd(const FInputActionValue& Value);

	void Power(const FInputActionValue& Value);

protected:
	virtual void PossessedBy(AController* NewController) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }
};
