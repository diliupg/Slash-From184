// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon( );

	void Equip( USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);

	void DeactivateEmbers( );

	void DisableSphereCollision( );

	void PlayEquipSound( );

	void AttachMeshToSocket( USceneComponent* InParent, FName SocketName );

	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay( ) override;

	UFUNCTION()
	void OnBoxOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	void ExecuteBoxHit( FHitResult& BoxHit ); 


	UFUNCTION( BlueprintImplementableEvent )
	void CreateFields( const FVector& FieldLocation ); 

private:

	void BoxTrace( FHitResult& BoxHit );

	UPROPERTY( EditAnywhere, Category = WeaponProperties )
	FVector BoxTraceExtent = FVector( 5.f );

	UPROPERTY( EditAnywhere, Category = WeaponProperties )
	bool bShowBoxDebug = false;

	UPROPERTY( EditAnywhere, Category = "Weapon Properties" )
	USoundBase* EquipSound;

	UPROPERTY( VisibleAnywhere, Category = "Weapon Properties" )
	UBoxComponent* WeaponBox;

	UPROPERTY( VisibleAnywhere )
	USceneComponent* BoxTraceStart;

	UPROPERTY( VisibleAnywhere )
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties" )
	float Damage = 20.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox( ) const { return WeaponBox; }
};
