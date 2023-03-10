// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/AttributeComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h" 
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

// Sets default values
ASlashCharacter::ASlashCharacter():
	MaxAngle( -45.f ),
	MinAngle( 45.f )
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false; 
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement( )->bOrientRotationToMovement = true;
	GetCharacterMovement( )->RotationRate = FRotator( 0.f, 400.f, 0.f );

	GetMesh( )->SetCollisionObjectType( ECollisionChannel::ECC_WorldDynamic );
	GetMesh( )->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	GetMesh( )->SetCollisionResponseToChannel( ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block );
	GetMesh( )->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap );
	GetMesh( )->SetGenerateOverlapEvents( true );

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( GetRootComponent( ) );
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "ViewCamera" ) );
	ViewCamera->SetupAttachment( CameraBoom );
}

void ASlashCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	if ( UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>( PlayerInputComponent ) )
	{
		EnhancedInputComponent->BindAction( MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move );
		EnhancedInputComponent->BindAction( LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look );
		EnhancedInputComponent->BindAction( JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump );
		EnhancedInputComponent->BindAction( EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed );
		EnhancedInputComponent->BindAction( AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack );
		//EnhancedInputComponent->BindAction( DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge );
	}
}

float ASlashCharacter::TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser )
{
	HandleDamage( DamageAmount );
	SetHUDHealth( );

	return DamageAmount;
}

void ASlashCharacter::SetHUDHealth( )
{
	if ( SlashOverlay && Attributes )
	{
		SlashOverlay->SetHealthBarPercent( Attributes->GetHealthPercent( ) );
	}
}

void ASlashCharacter::GetHit_Implementation( const FVector& ImpactPoint, AActor* Hitter )
{
	Super::GetHit_Implementation( ImpactPoint, Hitter );

	SetWeaponCollisionEnabled( ECollisionEnabled::NoCollision );

	if ( Attributes && Attributes->GetHealthPercent( ) > 0.f )
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add( FName( "EngagebleTarget" ) );
	
	GetPlayerController( );

	// == my code to limit camera pitch ==
	SetCameraPitchAngles( );

	InitializeSlashOverlay( );
}

void ASlashCharacter::InitializeSlashOverlay( )
{
	if ( PlayerController )
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>( PlayerController->GetHUD( ) );
		if ( SlashHUD )
		{
			SlashOverlay = SlashHUD->GetSlashOverlay( );
			if ( SlashOverlay && Attributes )
			{
				SlashOverlay->SetHealthBarPercent( Attributes->GetHealthPercent( ) );
				SlashOverlay->SetStaminaBarPercent( 1.f );
				SlashOverlay->SetGold( 0.f );
				SlashOverlay->SetSouls( 0.f );
			}
		}
	}
}

void ASlashCharacter::SetCameraPitchAngles( )
{
	if ( PlayerController )
	{
		// set camera pitch
		PlayerController->PlayerCameraManager->ViewPitchMin = MaxAngle; // Use whatever values you want
		PlayerController->PlayerCameraManager->ViewPitchMax = MinAngle;

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PlayerController->GetLocalPlayer( ) );
		if ( Subsystem )
		{
			Subsystem->AddMappingContext( SlashContext, 0 );
		}
	}
}

void ASlashCharacter::GetPlayerController( )
{
	PlayerController = Cast<APlayerController>( GetController( ) );
}

void ASlashCharacter::Move( const FInputActionValue& Value )
{
	if ( ActionState != EActionState::EAS_Unoccupied ) return;

	const FVector2D MovementVector = Value.Get<FVector2D>( );

	const FRotator Rotation = Controller->GetControlRotation( );
	const FRotator YawRotation( 0.f, Rotation.Yaw, 0.f );

	const FVector ForwardDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
	AddMovementInput( ForwardDirection, MovementVector.Y );
	const FVector RightDirection = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
	AddMovementInput( RightDirection, MovementVector.X );
}

void ASlashCharacter::Look( const FInputActionValue& Value )
{
	if ( ActionState != EActionState::EAS_Unoccupied ) return; 

	const FVector2D LookAxisVector = Value.Get<FVector2D>( );

	AddControllerPitchInput( LookAxisVector.Y );
	AddControllerYawInput( LookAxisVector.X );
}

void ASlashCharacter::Jump( )
{
	if ( IsUnOccupied( ) )
	{
		Super::Jump( );
	}
	
}
	 
bool ASlashCharacter::IsUnOccupied( )
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ASlashCharacter::EKeyPressed( )
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>( OverlappingItem );
	if ( OverlappingWeapon )
	{
		EquipWeapon( OverlappingWeapon );
	}
	else
	{
		if ( CanDisarm( ) )
		{
			Disarm( );
		}
		else if ( CanArm( ) )
		{
			Arm( );
		}
	}
}

void ASlashCharacter::EquipWeapon( AWeapon* Weapon )
{
	Weapon->Equip( GetMesh( ), FName( "RightHandSocket" ), this, this );
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

bool ASlashCharacter::CanAttack( )
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Attack( )
{	
	Super::Attack( );

	if ( CanAttack())
	{
		PlayAttackMontage( );
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackEnd( )
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanDisarm( )
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm( )
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon ;
}

void ASlashCharacter::Disarm( )
{
	PlayEqipMontage( FName( "Disarm" ) );
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm( )
{
	PlayEqipMontage( FName( "Arm" ) );
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::PlayEqipMontage( const FName SectionName )
{
	UAnimInstance* AnimInstance = GetMesh( )->GetAnimInstance( );
	if ( AnimInstance && EquipMontage )
	{
		AnimInstance->Montage_Play( EquipMontage );
		AnimInstance->Montage_JumpToSection( SectionName, EquipMontage );
	}
}

void ASlashCharacter::Die( )
{
	Super::Die( );

	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision( ); // inherited function we implemented in BaseCharacter
}

void ASlashCharacter::AttachWeaponToHand( ) // attach sword to hand socket
{
	if ( EquippedWeapon )
	{
		EquippedWeapon->AttachMeshToSocket( GetMesh( ), FName( "RightHandSocket" ) );
	}
}

void ASlashCharacter::AttachWeaponToBack( ) // attach sword to spine socket
{
	if ( EquippedWeapon )
	{
		EquippedWeapon->AttachMeshToSocket( GetMesh( ), FName( "SpineSocket" ) );
	}
}

void ASlashCharacter::FinishEquipping( )
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd( )
{
	ActionState = EActionState::EAS_Unoccupied;
}

