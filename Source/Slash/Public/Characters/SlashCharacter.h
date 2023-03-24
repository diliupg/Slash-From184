// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" 
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class ASoul;
class UAnimMontage;
class USlashOverlay;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY() 

public:

	ASlashCharacter();
	virtual void Tick( float DeltaTime ) override;

	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser ) override;

	virtual void GetHit_Implementation( const FVector& ImpactPoint, AActor* Hitter ) override;

	virtual void SetOverlappingItem( AItem* Item ) override;

	virtual void AddSouls( ASoul* Soul ) override;

	virtual void AddGold( ATreasure* Treasure ) override;

protected:

	virtual void BeginPlay( ) override;

	void InitializeSlashOverlay( );

	void SetCameraPitchAngles( );

	void GetPlayerController( );

	UPROPERTY( EditAnywhere )
	float MaxAngle;
	UPROPERTY( EditAnywhere )
	float MinAngle;

	/* Callback for Input */
	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputMappingContext> SlashContext;

	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> LookAction;
	 
	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> EKeyAction;

	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY( EditAnywhere, Category = Input )
	TObjectPtr<UInputAction> DodgeAction;

	void Move( const FInputActionValue& Value );
	void Look( const FInputActionValue& Value );
	virtual void Jump( ) override;

	bool IsUnOccupied( );

	void EKeyPressed( );

	void EquipWeapon( AWeapon* Weapon );

	virtual bool CanAttack( ) override;
	virtual void Attack( ) override;	 	
	void Dodge( );

	bool HasEnoughStamina( );

	bool IsOccupied( );

	virtual void AttackEnd( ) override;
	virtual void DodgeEnd( ) override;

	bool CanDisarm();
	bool CanArm( );
	void Disarm( );
	void Arm( );
	void PlayEqipMontage( const FName SectionName );
	virtual void Die( ) override;

	UFUNCTION( BlueprintCallable )
	void AttachWeaponToHand( );

	UFUNCTION(BlueprintCallable )
	void AttachWeaponToBack();

	UFUNCTION( BlueprintCallable )
	void FinishEquipping( );

	UFUNCTION( BlueprintCallable )
	void HitReactEnd( );
	
private:

	void SetHUDHealth( );

	/* Character Components */
	UPROPERTY( VisibleAnywhere )
	USpringArmComponent* CameraBoom;

	UPROPERTY( VisibleAnywhere )
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY( EditDefaultsOnly, Category = Montages )
	UAnimMontage* EquipMontage; // Arm and Disarm

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY( BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY( )
	USlashOverlay* SlashOverlay;

	UPROPERTY()
	APlayerController* PlayerController;

public:

	FORCEINLINE ECharacterState GetCharacterState( ) const { return CharacterState; }
	FORCEINLINE EActionState GetActionState( ) const { return ActionState; }
};
