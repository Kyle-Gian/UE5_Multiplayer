// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"

#include "BlasterComponents/CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverHeadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ABlasterCharacter, overlappingWeapon, COND_OwnerOnly);
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this;
	}
}

// Called when the game starts or when spawned
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called to bind functionality to input
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ABlasterCharacter::LookUp);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
}


void ABlasterCharacter::MoveForward(float value)
{
	if (Controller != nullptr  && value != 0.f)
	{
		const FRotator yawRotation(0.f, Controller->GetControlRotation().Yaw,0.f);
		const FVector direction(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(direction, value);
	}
}

void ABlasterCharacter::MoveRight(float value)
{
	const FRotator yawRotation(0.f, Controller->GetControlRotation().Yaw,0.f);
	const FVector direction(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(direction, value);
}

void ABlasterCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABlasterCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ABlasterCharacter::EquipButtonPressed()
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(overlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

// Called every frame
void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ABlasterCharacter::OnRep_OverLappingWeapon(AWeapon* LastWeapon)
{
	if (overlappingWeapon)
	{
		overlappingWeapon->ShowPickUpWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickUpWidget(false);
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(overlappingWeapon);
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

void ABlasterCharacter::SetOverLappingWeapon(AWeapon* Weapon)
{
	if (overlappingWeapon)
	{
		overlappingWeapon->ShowPickUpWidget(false);
	}
	
	overlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (overlappingWeapon)
		{
			overlappingWeapon->ShowPickUpWidget(true);
		}
	}
}


