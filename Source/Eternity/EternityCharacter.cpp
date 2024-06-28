// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternityCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AEternityCharacter

AEternityCharacter::AEternityCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...

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

	//std::numeric_limits<float>::max()
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 5000.f, 0.0f);

	//Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)return;
	
	PlayerController->SetShowMouseCursor(true);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	const UChildActorComponent* WeaponRangedChild = FindComponentByClass<UChildActorComponent>();
	if (WeaponRangedChild == nullptr) UE_LOG(LogTemp, Warning, TEXT("No ranged weapon child found"));
	
	AActor* WeaponActor = WeaponRangedChild->GetChildActor();
	RangedWeapon = Cast<ARangedWeapon>(WeaponActor);
	if (RangedWeapon == nullptr) UE_LOG(LogTemp, Warning, TEXT("The ranged weapon child isn't the right class"));
}

void AEternityCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AimAssist();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEternityCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Move);
		EnhancedInputComponent->BindAction(AimMouseAction, ETriggerEvent::Triggered, this, &AEternityCharacter::AimMouse);
		EnhancedInputComponent->BindAction(AimGamepadAction, ETriggerEvent::Triggered, this, &AEternityCharacter::AimGamepad);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Dodge);

		//Action
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Melee);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Started, this, &AEternityCharacter::RangedStart);
		EnhancedInputComponent->BindAction(RangedAction, ETriggerEvent::Completed, this, &AEternityCharacter::RangedEnd);
		EnhancedInputComponent->BindAction(PowerAction, ETriggerEvent::Triggered, this, &AEternityCharacter::Power);
	}
}

void AEternityCharacter::AimAssist()
{
	if (!bIsShoot) return;

	ClosestEnemy = nullptr;
	
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> Enemies;
	TArray<FHitResult> OutHits;
	
	const FVector Start = GetActorLocation() + GetActorForwardVector() * DeltaStartBox;
	const FVector End = GetActorLocation() + GetActorForwardVector() * DeltaEndBox;
	
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Start,	End, BoxSize, GetActorRotation(),
		TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHits, true);

	for (int i=0; i < OutHits.Num(); i++)
	{
		if (OutHits[i].GetActor()->IsA(EnemyClass))
		{
			Enemies.Add(OutHits[i].GetActor());
		}
	}

	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (i == 0)
		{
			ClosestEnemy = Enemies[i];
		}
		else
		{
			const double CurrentEnemyDist = ClosestEnemy->GetActorLocation().DistSquared(ClosestEnemy->GetActorLocation(), GetActorLocation());
			const double EnemyDist = Enemies[i]->GetActorLocation().DistSquared(Enemies[i]->GetActorLocation(), GetActorLocation());

			if (EnemyDist < CurrentEnemyDist)
			{
				ClosestEnemy = Enemies[i];
			}
		}
	}
}

void AEternityCharacter::AimMouse(const FInputActionValue& Value)
{
	if (!bIsShoot || !PlayerController) return;
	
	FVector2D PlayerScreenPosition;
	const FVector2D MousePosition = GetMousePosition();
	const FVector PlayerLocation = GetActorLocation();

	PlayerController->ProjectWorldLocationToScreen(PlayerLocation, PlayerScreenPosition);

	// todo: need to be improve for follow exactly the player mouse
	// Calculate the direction vector from the player to the mouse position
	const FVector2D Direction = MousePosition - PlayerScreenPosition;
	
	// Calculate the angle in radians and convert to degrees
	const float AngleRad = FMath::Atan2(Direction.Y, Direction.X);
	const float YawAngle = FMath::RadiansToDegrees(AngleRad);
	
	// Set the player's yaw rotation to face the mouse
	const FRotator NewRotation = FRotator(0.0f, YawAngle + 90.f, 0.0f);
	SetActorRotation(NewRotation);
}

void AEternityCharacter::AimGamepad(const FInputActionValue& Value)
{
	if (!bIsShoot) return;
	
	const FVector2D GamepadPosition = Value.Get<FVector2D>();
	const float AngleRad = FMath::Atan2(GamepadPosition.Y, GamepadPosition.X);
	const float YawAngle = FMath::RadiansToDegrees(AngleRad);
	const FRotator NewRotation = FRotator(0.f, YawAngle + 90.f, 0.f);

	SetActorRotation(NewRotation);
}

void AEternityCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr || bShouldMove) return;
	
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
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
}

void AEternityCharacter::RangedEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("RangedEnd"));
	bIsShoot = false;
}

void AEternityCharacter::Power(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Power"));
}

FVector2D AEternityCharacter::GetMousePosition() const
{
	if (PlayerController)
	{
		FVector2D MousePosition;
		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			return MousePosition;
		}
	}
	return FVector2D::ZeroVector;
}

void AEternityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// }
}
