// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
 
UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick( float DeltaTime ) override;

	void CheckPatrolTarget( );

	void CheckCombatTarget( );

	virtual void GetHit_Implementation( const FVector& ImpactPoint ) override;

	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser ) override;

	virtual void Destroyed( ) override;

private:

	UPROPERTY( VisibleAnywhere )
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY( VisibleAnywhere )    
	UPawnSensingComponent* PawnSensing;  

	UPROPERTY( EditAnywhere )
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere )
	double CombatRadius = 750.f;

	UPROPERTY( EditAnywhere )
	double AttackRadius = 150.f;

	/*
	* Navigation
	*/

	UPROPERTY()
	class AAIController* EnemyController;

	// current patrol target
	UPROPERTY( EditInstanceOnly, Category = AINavigation )
	AActor* PatrolTarget;

	UPROPERTY( EditInstanceOnly, Category = AINavigation )
	TArray<AActor*> PatrolTargets;

	UPROPERTY( EditAnywhere )
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished( );
	
	UPROPERTY(EditAnywhere, Category = AINavigation )
	float WaitMin = 5.f;
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float WaitMax = 10.f;
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float WalkSpeed = 150.f;
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float RunSpeed = 300.f;
	
	/**AI Behavior*/
	void HideHealthBar();
	void ShowHealthBar( );
	void LoseInterest( );
	void StartPatrolling( );
	void ChaseTarget( );


	bool IsOutsideCombatRadius( );
	bool IsOutsideAttackRadius( );
	bool IsInsideAttackRadius( );
	bool IsChasing( );
	bool IsAttacking( );
	bool IsDead( );
	bool IsEngaged( );

	void ClearPatrolTimer( );

	/* Combat */
	void StartAttackTimer( );
	void ClearAttackTimer( );

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat )
	float AttackMin = 0.5f;
	UPROPERTY( EditAnywhere, Category = Combat )
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat )
	float PatrollingSpeed = 125.f;
	UPROPERTY( EditAnywhere, Category = Combat )
	float chaseSpeed = 300.f;

protected:

	virtual void BeginPlay() override;
	 
	virtual void Die( ) override;
	bool InTargetRange( AActor* Target, double Radius );
	void MoveToTarget( AActor* Target );
	AActor* ChoosePatrolTarget( );
	virtual void Attack( ) override;
	virtual bool CanAttack( ) override; 
	virtual void HandleDamage( float DamageAmount ) override ;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(EditAnywhere, Category = Combat )
	float DeathLifespan = 8.f;

	UFUNCTION()
	void PawnSeen( APawn* SeenPawn );
 
	UPROPERTY(BlueprintReadOnly )
	TEnumAsByte<EDeathPose> DeathPose ;

	UPROPERTY( BlueprintReadOnly )
	EEnemyState  EnemyState = EEnemyState::EES_Patrolling;

public:	

	

};
