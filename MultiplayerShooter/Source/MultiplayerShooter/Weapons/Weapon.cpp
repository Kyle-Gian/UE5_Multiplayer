// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MultiplayerShooter/BlasterCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(weaponMesh);

	weaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	weaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	areaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	areaSphere->SetupAttachment(RootComponent);
	areaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	areaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	pickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	pickUpWidget->SetupAttachment(RootComponent);

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1,45.0f, FColor::Red,"Has Auth");

		areaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		areaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		areaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	}
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (pickUpWidget)
	{
		pickUpWidget->SetVisibility(false);
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	ABlasterCharacter* blasterCharacter = Cast<ABlasterCharacter>(_otherActor);
	GEngine->AddOnScreenDebugMessage(-1,15.0f, FColor::Red,"Collision Detected");
	if (blasterCharacter && pickUpWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.0f, FColor::Red,"DisplayText");
		pickUpWidget->SetVisibility(true);
	}
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

