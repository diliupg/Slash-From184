// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributeComponent();

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = ActorAttributes )
	float Health;

	UPROPERTY( EditAnywhere, Category = ActorAttributes )
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = ActorAttributes )
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = ActorAttributes )
	int32 Souls;


public:

	void ReceiveDamage( float Damage );
	float GetHealthPercent( );
	bool IsAlive( );
	void AddGold( int32 AmountOfGold);
	void AddSouls( int32 NumberOfSouls );
	FORCEINLINE int32 GetGold( ) const { return Gold; }
	FORCEINLINE int32 GetSouls( ) const { return Souls; }
	
};
