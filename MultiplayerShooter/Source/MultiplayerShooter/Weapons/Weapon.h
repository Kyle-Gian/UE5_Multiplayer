// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class MULTIPLAYERSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor,
		UPrimitiveComponent* _otherComp,
		int32 _otherBodyIndex, bool _bFromSweep,
		const FHitResult& _sweepResult);
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* weaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* areaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaaponState weaponState;
	
		UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    	class UWidgetComponent* pickUpWidget;
public:	


};
