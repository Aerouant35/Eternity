// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternityCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "TranscendentFormComponent.h"
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

	TranscendentFormComponent = CreateDefaultSubobject<UTranscendentFormComponent>(TEXT("TranscendentFormComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AEternityCharacter::BeginPlay()
{
	Super::BeginPlay();

	//std::numeric_limits<float>::max()
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 5000.f, 0.0f);

	//Add Input Mapping Context
	GetPlayerController()->SetShowMouseCursor(true);
	if (EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	TranscendentFormComponent->Init(this);
	HealthComponent->Init();
	
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

APlayerController* AEternityCharacter::GetPlayerController() const
{
	return CastChecked<APlayerController>(Controller, ECastCheckedType::NullAllowed);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEternityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void AEternityCharacter::EnableEnhancedInput(const bool Enable) const
{
	if (Enable)
		EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	else
		EnhancedInputSubsystem->RemoveMappingContext(DefaultMappingContext);
}

void AEternityCharacter::AimMouse(const FInputActionValue& Value)
{
	if (!bIsShoot || !GetPlayerController()) return;
	
	FVector2D PlayerScreenPosition;
	const FVector2D MousePosition = GetMousePosition();
	const FVector PlayerLocation = GetActorLocation();

	GetPlayerController()->ProjectWorldLocationToScreen(PlayerLocation, PlayerScreenPosition);

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

	if (GetWorldTimerManager().IsTimerActive(DodgeTimerHandle)) GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
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
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("RangedStart"));
	bIsShoot = true;

	if (TranscendentFormComponent->bIsTranscendentFormActive)
		TranscendentFormComponent->StartShoot();
	else
		RangedWeapon->StartShoot();
}

void AEternityCharacter::RangedEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("RangedEnd"));
	bIsShoot = false;
}

void AEternityCharacter::Power(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Power"));

	if (TranscendentFormComponent->bIsTranscendentFormActive)
		TranscendentFormComponent->Finisher();
	else
		TranscendentFormComponent->ActivateTranscendentForm();
}

FVector2D AEternityCharacter::GetMousePosition() const
{
	if (GetPlayerController())
	{
		FVector2D MousePosition;
		if (GetPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			return MousePosition;
		}
	}
	return FVector2D::ZeroVector;
}

void AEternityCharacter::SetWeaponsActive(const bool Active)
{
	if (RangedWeapon == nullptr) return;
	RangedWeapon->SetActive(Active);
}


void AEternityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// }
}
