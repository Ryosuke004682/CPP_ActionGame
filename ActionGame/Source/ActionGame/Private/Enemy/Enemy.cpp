// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Component/PlayerComponent.h"
#include "HUD/HitpointBarComponent.h"
#include "Items/Weapons/Weapon.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility , ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera , ECollisionResponse::ECR_Ignore);

	PlayerComponent = CreateDefaultSubobject<UPlayerComponent>(TEXT("Attribute"));
	HealthBarWidget = CreateDefaultSubobject<UHitpointBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationPitch = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);

	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	return DamageAmount;
}


void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();

	if (IsAlive()) { DirectionalHitReact(ImpactPoint); }
	else { Die(); }

	PlayerHitSound(ImpactPoint);
	PlayerSlashSound(ImpactPoint);

	/*パーティクルの大きさと、ランダムで角度を変えて出す演出*/
	FVector Scale(2.5f, 2.5f, 2.5f);
	FRotator RandomRotation
		= FRotator
		(
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f)
		);

	SpawnHitParticles(ImpactPoint, Scale, RandomRotation);
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this , &AEnemy::PawnSeen);
	}

	InitializeEnemy();
}


void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	PlayDeathMontage();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}


void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;

	Super::Attack();
	PlayAttackMontage();
}


bool AEnemy::CanAttack()
{

	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}


void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}


void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (PlayerComponent && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(PlayerComponent->GetHealthPercent());//体力を減らす
	}
}


int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}

	return Selection;
}


void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();//武器を装備させたい場合の処理。（Zonbieと、虫は例外）
}


void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, CombatRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);

		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}


void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInTarget();

		if (!IsEngaged()) { StartPatrolling(); }

	}
	else if (IsOutsideAttackRange() && !IsChasing())
	{
		ClearAttackTimer();

		if (!IsEngaged()) { ChaseTarget(); }

	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}


void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);

}


void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}


void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}


void AEnemy::LoseInTarget()
{
	CombatTarget = nullptr;
	HideHealthBar();
}


void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}


void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}


bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}


bool AEnemy::IsOutsideAttackRange()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}


bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget , AttackRadius);
}


bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}


bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}


bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;;
}


bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}


void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}


void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin , AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer , this , &AEnemy::Attack , AttackTime);
}


void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}


bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = ( Target -> GetActorLocation() - GetActorLocation() ).Size();
	return DistanceToTarget <= Radius;
}


void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
}


AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;

	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}


void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass) {
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

		EquippedWeapon = DefaultWeapon;
	}
}


void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead      &&
		EnemyState != EEnemyState::EES_Chasing   &&
		EnemyState  < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("Player"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}