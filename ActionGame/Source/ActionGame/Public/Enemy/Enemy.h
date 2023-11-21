// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterType.h"
#include "Enemy.generated.h"


class UHitpointBarComponent;
class UPawnSensingComponent;


UCLASS()
class ACTIONGAME_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()


public:
	AEnemy();

	/*<AActor>*/
	virtual void  Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void  Destroyed() override;

	UPROPERTY(VisibleAnywhere)
		float EnemyDisappear = 5.f;//敵が消えるまでの時間を定義
	/*</AActor>*/


	/*<IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint);
	/*</IHitInterface>*/

protected:
	/*<AActor>*/
	virtual void BeginPlay() override;
	void InitializeEnemy();
	/*</AActor>*/

	/*<ABaseCharacter>*/
	virtual void Die() override;
	virtual void Attack()override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;
	/*</ABaseCharacter>*/


	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;
private:
	
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();//HPバーを隠す処理
	void ShowHealthBar();//HPバーを見せる処理
	void LoseInTarget();//見失う処理
	void StartPatrolling();//巡回の処理
	void ChaseTarget();//ターゲットを追いかける処理
	bool IsOutsideCombatRadius();//戦闘範囲外かどうか
	bool IsOutsideAttackRange();//攻撃範囲外かどうか
	bool IsInsideAttackRadius();//攻撃範囲内かどうか
	bool IsChasing();//チェイス中かどうか
	bool IsAttacking();//攻撃中かどうか
	bool IsDead();//死んでるかどうか
	bool IsEngaged();//ついてきてるかどうか
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
		UHitpointBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
		double CombatRadius = 1000.f;//追跡距離

	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.f;//攻撃範囲（攻撃射程）

	UPROPERTY()
		class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 2.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 1.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 8.f;
};
