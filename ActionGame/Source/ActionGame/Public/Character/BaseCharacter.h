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


	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual bool CanAttack();

	bool IsAlive();

	/*Play montage function*/
			void PlayHitReactMontage(const FName& SectionName);
			void DirectionalHitReact(const FVector& ImpactPoint);
			void PlayerHitSound     (const FVector& ImpactPoint);
			void PlayerSlashSound   (const FVector& ImpactPoint);
			void SpawnHitParticles  (const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
			void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

			int32 PlayRandomMontageSection(UAnimMontage* Montage , const TArray<FName>& SectionNames);
			virtual int32 PlayAttackMontage();
			virtual int32 PlayDeathMontage();
					 void DisableCapsule();


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
	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Sounds)
		USoundBase* SlashSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		UParticleSystem* HitParticles;
};
