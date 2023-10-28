// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/CharacterType.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UPlayerComponent;
class UHitpointBarComponent;



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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
		float EnemyDisappear = 5.f;


private:

	UPROPERTY(VisibleAnywhere)
		UPlayerComponent* PlayerComponent;

	UPROPERTY(VisibleAnywhere)
		UHitpointBarComponent* HealthBarWidget;

	/*Animation Montages*/
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* SlashSound;

	UPROPERTY(EditAnywhere, Category = Effect)
		UParticleSystem* HitParticles;

	UPROPERTY()
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 500.0f;

protected:
	virtual void BeginPlay() override;

	void Die();

	/* Play Montage Function */
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

};
