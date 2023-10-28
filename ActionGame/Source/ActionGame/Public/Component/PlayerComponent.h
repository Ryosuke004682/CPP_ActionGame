// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:

	//åªç›ÇÃëÃóÕ
	UPROPERTY(EditAnywhere , Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere , Category = "Actor Attributes")
	float MaxHealth;


public:
	void ReceiveDamage(float Damage);
	float GetHealthPercent();

	bool IsAlive();
};
