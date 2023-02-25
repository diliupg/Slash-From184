// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::ReceiveDamage( float Damage )
{
	Health = FMath::Clamp( Health - Damage, 0.f, MaxHealth );

}

float UAttributeComponent::GetHealthPercent( )
{
	return Health / MaxHealth;
}

bool UAttributeComponent::IsAlive( )
{
	return Health > 0.f;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

