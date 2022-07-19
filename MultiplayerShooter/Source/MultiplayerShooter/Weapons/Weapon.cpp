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
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		areaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		areaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		areaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		areaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
	
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
	if (blasterCharacter)
	{
		blasterCharacter->SetOverLappingWeapon(this);
		GEngine->AddOnScreenDebugMessage(-1,15.0f, FColor::Red,"DisplayText");
	}
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
	UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	ABlasterCharacter* blasterCharacter = Cast<ABlasterCharacter>(_otherActor);
	if (blasterCharacter)
	{
		blasterCharacter->SetOverLappingWeapon(nullptr);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ShowPickUpWidget(bool bShowWidget)
{
	if (pickUpWidget)
	{
		pickUpWidget->SetVisibility(bShowWidget);
	}
}