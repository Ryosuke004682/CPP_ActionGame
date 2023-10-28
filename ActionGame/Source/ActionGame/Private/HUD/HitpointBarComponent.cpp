// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HitpointBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHitpointBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}


}
