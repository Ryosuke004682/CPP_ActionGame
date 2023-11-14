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
class UPawnSensingComponent;


UCLASS()
class ACTIONGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
			void CheckPatrolTarget();
			void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint);
			void DirectionalHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
		float EnemyDisappear = 5.f;


private:

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
		UPlayerComponent* PlayerComponent;

	UPROPERTY(VisibleAnywhere)
		UHitpointBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;


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


	/*Navigation*/
	UPROPERTY()
		class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitMax = 2.f;


	EEnemyState EnemyState = EEnemyState::EES_Patrolling;


protected:
	virtual void BeginPlay() override;

	void Die();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget (AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);


	/* Play Montage Function */
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

};
