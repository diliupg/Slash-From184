// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{ 
	PrimaryActorTick.bCanEverTick = true;
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>( TEXT( "Attributes" ) );
	GetCapsuleComponent( )->SetCollisionResponseToChannel( ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore ); 
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation( const FVector& ImpactPoint, AActor* Hitter )
{
	if ( IsAlive( ) && Hitter)
	{
		DirectionalHitReact( Hitter->GetActorLocation() );
	}
	else Die( );

	PlayHitSound( ImpactPoint );
	SpawnHitParticles( ImpactPoint );
}

void ABaseCharacter::Attack( )
{

}

void ABaseCharacter::Die( )
{

}

void ABaseCharacter::PlayHitReactMontage( const FName SectionName )
{
	UAnimInstance* AnimInstance = GetMesh( )->GetAnimInstance( );
	if ( AnimInstance && HitReactMontage )
	{
		AnimInstance->Montage_Play( HitReactMontage );
		AnimInstance->Montage_JumpToSection( SectionName, HitReactMontage );
	}
}

void ABaseCharacter::DirectionalHitReact( const FVector& ImpactPoint )
{
	const FVector Forward = GetActorForwardVector( );
	// lower impact point to the enemy's ActorLocation.Z
	const FVector ImpactLowered( ImpactPoint.X, ImpactPoint.Y, GetActorLocation( ).Z );
	const FVector ToHit = (ImpactLowered - GetActorLocation( )).GetSafeNormal( ); // normalized 

	//Forward * ToHit = |Forward||ToHit|*cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct( Forward, ToHit );
	// take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos( CosTheta );
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees( Theta );

	// if cross product points down, theta should be negative
	const FVector CrossProduct = FVector::CrossProduct( Forward, ToHit );
	if ( CrossProduct.Z < 0 )
	{
		Theta *= -1.f;
	}
	FName Section( "FromBack" );

	if ( Theta >= -45.f && Theta < 45.f )
	{
		Section = ("FromFront");
	}
	else if ( Theta >= -135.f && Theta < -45.f )
	{
		Section = ("FromLeft");
	}
	else if ( Theta >= 45.f && Theta < 135.f )
	{
		Section = ("FromRight");
	}

	PlayHitReactMontage( Section );
}

void ABaseCharacter::PlayHitSound( const FVector& ImpactPoint )
{
	if ( HitSound )
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles( const FVector& ImpactPoint )
{
	if ( HitParticles )
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld( ),
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage( float DamageAmount )
{
	if ( Attributes )
	{
		Attributes->ReceiveDamage( DamageAmount );
	} 
}

void ABaseCharacter::PlayMontageSection( UAnimMontage* Montage, const FName& SectionName )
{
	UAnimInstance* AnimInstance = GetMesh( )->GetAnimInstance( );
	if ( AnimInstance && Montage )
	{
		AnimInstance->Montage_Play( Montage );
		AnimInstance->Montage_JumpToSection( SectionName, Montage );
	}
}

int32 ABaseCharacter::PlayRandomMontageSection( UAnimMontage* Montage, const TArray<FName>& SectionNames )
{
	if ( SectionNames.Num( ) <= 0 ) return -1;
	const int32 MaxSectionIndex = SectionNames.Num( ) - 1;
	const int32 Selection = FMath::RandRange( 0, MaxSectionIndex );
	PlayMontageSection( Montage, SectionNames[Selection] );
	return Selection;
}

int32 ABaseCharacter::PlayAttackMontage( )
{
	return PlayRandomMontageSection( AttackMontage, AttackMontageSections );
}

int32 ABaseCharacter::PlayDeathMontage( )
{
	return PlayRandomMontageSection( DeathMontage, DeathMontageSections );
}

void ABaseCharacter::StopAttackMontage( )
{
	UAnimInstance* AnimInstance = GetMesh( )->GetAnimInstance( );
	if ( AnimInstance )
	{
		AnimInstance->Montage_Stop( 0.2f, AttackMontage );
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget( )
{
	if ( CombatTarget == nullptr ) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation( );
	const FVector Location = GetActorLocation( );

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotatinWarpTarget( )
{
	if ( CombatTarget )
	{
		return CombatTarget->GetActorLocation( );
	}
	return FVector( );
}

void ABaseCharacter::DisableCapsule( )
{
	GetCapsuleComponent( )->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

bool ABaseCharacter::CanAttack( )
{
	return false;
}

bool ABaseCharacter::IsAlive( )
{
	return Attributes && Attributes->IsAlive( );
}

void ABaseCharacter::AttackEnd( )
{

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled( ECollisionEnabled::Type CollisionEnabled )
{
	if ( EquippedWeapon && EquippedWeapon->GetWeaponBox( ) )
	{
		EquippedWeapon->GetWeaponBox( )->SetCollisionEnabled( CollisionEnabled );
		EquippedWeapon->IgnoreActors.Empty( );
	}
}

