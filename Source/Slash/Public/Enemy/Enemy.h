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
	AEnemy( );

	/* <AActor> */
	virtual void Tick( float DeltaTime ) override;
	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser ) override;
	virtual void Destroyed( ) override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation( const FVector& ImpactPoint, AActor* Hitter ) override;
	/* </IHitInterace> */

protected:
	/* <AActor> */
	virtual void BeginPlay( ) override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual void Die_Implementation( ) override;

	void SpawnSoul( );

	virtual void Attack( ) override;
	virtual bool CanAttack( ) override;
	virtual void AttackEnd( ) override;
	virtual void HandleDamage( float DamageAmount ) override;
	/* </ABaseCharacter> */

	UPROPERTY( BlueprintReadOnly )
	EEnemyState  EnemyState = EEnemyState::EES_Patrolling;

	
private:

	/* AIBehavior */
	void InitializeEnemy( );
	void CheckPatrolTarget( );
	void CheckCombatTarget( );
	void PatrolTimerFinished( );
	void HideHealthBar( );
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
	void StartAttackTimer( );
	void ClearAttackTimer( );
	bool InTargetRange( AActor* Target, double Radius );
	void MoveToTarget( AActor* Target );
	AActor* ChoosePatrolTarget( );
	void SpawnDefaultWeapon( );

	UFUNCTION( )
	void PawnSeen( APawn* SeenPawn ); // callback to OnPawnSeen in UPawnSensingComponent

	UPROPERTY( VisibleAnywhere )
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY( VisibleAnywhere )
	UPawnSensingComponent* PawnSensing;

	UPROPERTY( EditAnywhere, Category = Combat )
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY( EditAnywhere, Category = Combat )
	double CombatRadius = 850.f;

	UPROPERTY( EditAnywhere, Category = Combat )
	double AttackRadius = 170.f;

	UPROPERTY( EditAnywhere, Category = Combat )
	double AcceptanceRadius = 60.f;

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
	
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float PatrolWaitMin = 3.f;
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float PatrolWaitMax = 6.f;

	UPROPERTY( EditAnywhere, Category = Combat )
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;
	
	UPROPERTY( EditAnywhere, Category = Combat )
	float AttackMin = 0.2f;
	UPROPERTY( EditAnywhere, Category = Combat )
	float AttackMax = 0.8f;

	UPROPERTY( EditAnywhere, Category = Combat )
	float chaseSpeed = 300.f;

	UPROPERTY( EditAnywhere, Category = Combat )
	float DeathLifespan = 8.f;

	UPROPERTY( EditAnywhere, Category = AINavigation )
	float WalkSpeed = 150.f;
	UPROPERTY( EditAnywhere, Category = AINavigation )
	float RunSpeed = 300.f;

	UPROPERTY( EditAnywhere, Category = Combat )
	TSubclassOf<class ASoul> SoulClass;
};
