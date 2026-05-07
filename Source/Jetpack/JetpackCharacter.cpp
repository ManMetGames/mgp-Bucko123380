// Copyright Epic Games, Inc. All Rights Reserved.

#include "JetpackCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Jetpack.h"
#include "GameFramework/CharacterMovementComponent.h"


AJetpackCharacter::AJetpackCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AJetpackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJetpackCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AJetpackCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJetpackCharacter::Look);
		
		// Jetpacking
		EnhancedInputComponent->BindAction(JetpackAction, ETriggerEvent::Started, this, &AJetpackCharacter::StartJetpack);
		EnhancedInputComponent->BindAction(JetpackAction, ETriggerEvent::Completed, this, &AJetpackCharacter::StopJetpack);
	}
	else
	{
		UE_LOG(LogJetpack, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AJetpackCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AJetpackCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AJetpackCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AJetpackCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AJetpackCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AJetpackCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}




void AJetpackCharacter::StartJetpack()
{
    bIsJetpacking = true;
    GetCharacterMovement()->SetMovementMode(MOVE_Flying); // switch to flying mode for jetpacking
}

void AJetpackCharacter::StopJetpack()
{
	bIsJetpacking = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling); // Switch back to falling mode when jetpacking stops
}

void AJetpackCharacter::HandleJetpack(float DeltaTime)
{
	if (bIsJetpacking)
	{
		if (CurrentFuel > 0)
		{
			// Apply upward force
			FVector JetpackThrust = FVector::UpVector * JetpackForce;
			GetCharacterMovement()->AddForce(JetpackThrust);
			// Consume fuel
			CurrentFuel -= FuelBurnRate * DeltaTime;
			if (CurrentFuel < 0.0f)
			{
				CurrentFuel = 0.0f;
				StopJetpack(); //if fuel reachs 0, stop jetpacking
			}
		}
		else
		{
			StopJetpack();
		}
	}
	else
	{
		// Recharge when idle
		if (CurrentFuel < MaxFuel) //recharge only if fuel is not full
		{
			CurrentFuel = FMath::Min(CurrentFuel + FuelRechargeRate * DeltaTime, MaxFuel);
		}
	}
}

void AJetpackCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleJetpack(DeltaTime);
}
