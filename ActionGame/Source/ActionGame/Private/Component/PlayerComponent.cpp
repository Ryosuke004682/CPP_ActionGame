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

void UPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

