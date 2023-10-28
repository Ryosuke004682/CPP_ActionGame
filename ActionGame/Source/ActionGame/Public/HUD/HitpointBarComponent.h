// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HitpointBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UHitpointBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
		class UHealthBar* HealthBarWidget;
};
