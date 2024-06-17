// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternityCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AEternityCharacter

AEternityCharacter::AEternityCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, -1.f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 5000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AEternityCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, std::numeric_limits<float>::max(), 0.0f); // ...at this rotation rate

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEternityCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEternityCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Move);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Dodge);

		//Action
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Melee);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Started, this, &AEternityCharacter::RangedStart);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Completed, this, &AEternityCharacter::RangedEnd);
		EnhancedInputComponent->BindAction(PowerAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Power);
	}
}

void AEternityCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (bShouldMove) return;
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEternityCharacter::Dodge(const FInputActionValue& Value)
{
	if (GetWorldTimerManager().GetTimerElapsed(DodgeTimerHandle) > 0.f || bShouldMove) return;

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Dodge"));

	GetWorldTimerManager().SetTimer(DodgeTimerHandle, DodgeCooldown, false);

	FVector DodgeDirection = FVector(GetActorForwardVector().X * DeltaDodge, GetActorForwardVector().Y * DeltaDodge, 0);
	LaunchCharacter(DodgeDirection, false, false);
}

void AEternityCharacter::Melee(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Melee"));
}

void AEternityCharacter::RangedStart(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("RangedStart"));

	bIsShoot = true;
	bShouldMove = true;
	
	if (RangedWeapon == nullptr) return;
	RangedWeapon->StartShoot();
}

void AEternityCharacter::RangedEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("RangedEnd"));

	bIsShoot = false;
	
	if (RangedWeapon == nullptr) return;
	RangedWeapon->EndShoot();
}

void AEternityCharacter::Power(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Power"));
}

void AEternityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// }
}
