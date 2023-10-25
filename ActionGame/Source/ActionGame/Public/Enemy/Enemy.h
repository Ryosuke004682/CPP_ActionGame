// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UPlayerComponent;

UCLASS()
class ACTIONGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint);
			void DirectionalHitReact(const FVector& ImpactPoint);

private:

	UPROPERTY(VisibleAnywhere)
		UPlayerComponent* PlayerComponent;

	/*Animation Montages*/
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* SlashSound;

	UPROPERTY(EditAnywhere, Category = Effect)
		UParticleSystem* HitParticles;


protected:
	virtual void BeginPlay() override;

	/*Play montage function*/
	void PlayHitReactMontage(const FName& SectionName);

};
