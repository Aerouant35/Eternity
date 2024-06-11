// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternityCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
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
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(RootComponent);
	// CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	// CameraBoom->TargetArmLength = 80000.0f; // The camera follows at this distance behind the character
	// CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	// CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a follow camera
	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	// Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AEternityCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1500.f, 0.0f); // ...at this rotation rate

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
		//Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Move);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Dodge);

		//Action
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Melee);
		//EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Started, this, &AEternityCharacter::RangedStart);
		// EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Completed, this, &AEternityCharacter::RangedEnd);
		EnhancedInputComponent->BindAction(PowerAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Power);

		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Look);
	}
}

void AEternityCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
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

// void AEternityCharacter::Look(const FInputActionValue& Value)
// {
	// return;

	// // input is a Vector2D
	// FVector2D LookAxisVector = Value.Get<FVector2D>();

	// if (Controller != nullptr)
	// {
		// // add yaw and pitch input to controller
		// // AddControllerYawInput(LookAxisVector.X);
		// // AddControllerPitchInput(LookAxisVector.Y);
	// }
// }

void AEternityCharacter::Dodge(const FInputActionValue& Value)
{
	// BlockInput(true);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Dodge"));

	GetCharacterMovement()->Velocity = FVector(GetActorForwardVector().X * DeltaDodge, GetActorForwardVector().Y * DeltaDodge, 0);
	// GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, AEternityCharacter::BlockInput, 1.f, false, 0);
}

void AEternityCharacter::Melee(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Melee"));
}

void AEternityCharacter::RangedStart(const FInputActionValue& Value)
{
	if (RangedWeapon == nullptr) return;
	RangedWeapon->StartShoot();
}

void AEternityCharacter::RangedEnd(const FInputActionValue& Value)
{
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
