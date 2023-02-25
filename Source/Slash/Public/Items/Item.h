// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem();

	virtual void Tick( float DeltaTime ) override;


protected:

	virtual void BeginPlay( ) override;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = SineParameters )
	float TimeConstant = 5.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = SineParameters )
	float Amplitude = 1.0f;

	UFUNCTION(BlueprintPure)
	float TransformedSin( );

	UFUNCTION( BlueprintPure )
	float TransformedCos( );

	template<typename T>
	T Avg( T First, T Second );

	UFUNCTION()
	virtual void OnSphereOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	UFUNCTION( )
	virtual void OnSphereEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	UStaticMeshComponent* ItemMesh;
	
	EItemState ItemState = EItemState::EIS_Hovering;
	
	UPROPERTY( VisibleAnywhere )
	USphereComponent* Sphere;

	UPROPERTY( VisibleAnywhere )
	class UNiagaraComponent* EmbersEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true" ))
	float RunningTime = 10.f;

	
};

template<typename T>
inline T AItem::Avg( T First, T Second )
{
	return (First + Second ) / 2;
}
