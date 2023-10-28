// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PlayerComponent.h"

UPlayerComponent::UPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPlayerComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage , 0.f , MaxHealth);
}

float UPlayerComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UPlayerComponent::IsAlive()
{
	return Health > 0.f;
}

void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

