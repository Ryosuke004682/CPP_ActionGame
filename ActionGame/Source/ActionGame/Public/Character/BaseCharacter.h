// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UPlayerComponent;
class UAnimMontage;

UCLASS()
class ACTIONGAME_API ABaseCharacter : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();
			void DirectionalHitReact(const FVector& ImpactPoint);
			void PlayerHitSound     (const FVector& ImpactPoint);
			void PlayerSlashSound   (const FVector& ImpactPoint);
<<<<<<< HEAD
			void SpawnHitParticles  (const FVector& ImpactPoint);
=======
			void SpawnHitParticles  (const FVector& ImpactPoint, FVector ParticleScale, FRotator RandomRotation);
			void DisableCapsule();
	virtual bool CanAttack();
			bool IsAlive();

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();


	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	/*Play montage function*/
			void PlayHitReactMontage(const FName& SectionName);

>>>>>>> master
	virtual void HandleDamage(float DamageAmount);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;


	/*Animation Montage*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere , Category = Combat)
		TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

	/*Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UPlayerComponent* PlayerComponent;


private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* SlashSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		UParticleSystem* HitParticles;
};
