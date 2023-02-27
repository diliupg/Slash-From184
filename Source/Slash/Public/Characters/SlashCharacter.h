// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" 
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	ASlashCharacter();
	virtual void Tick( float DeltaTime ) override;
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	UPROPERTY( EditAnywhere )
	float MaxAngle;
	UPROPERTY( EditAnywhere )
	float MinAngle;

	virtual void Jump( ) override;

	void EKeyPressed( );

protected:

	virtual void BeginPlay() override;

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

	virtual void Attack( ) override;

	/* 	* Play Montage Functions */
	 
	virtual void AttackEnd( ) override ;
	virtual bool CanAttack( ) override; 
	bool CanDisarm();
	bool CanArm( );
	void PlayEqipMontage(const FName SectionName);
	

	UFUNCTION(BlueprintCallable )
	void Disarm();

	UFUNCTION( BlueprintCallable )
	void Arm( );

	UFUNCTION( BlueprintCallable )
	void FinishEquipping( );
	 
private:

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

public:

	FORCEINLINE void SetOverlappingItem( AItem* Item ) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState( ) const { return CharacterState; }
};
