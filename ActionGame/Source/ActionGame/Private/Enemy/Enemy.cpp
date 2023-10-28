// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActionGame/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Component/PlayerComponent.h"
#include "HUD/HitpointBarComponent.h"


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

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0 , 3);
		FName SectionName = FName();

		switch (Selection)
		{
			case 0:
				SectionName = FName("Death1");
				DeathPose = EDeathPose::EDP_Death1;
				break;

			case 1:
				SectionName = FName("Death2");
				DeathPose = EDeathPose::EDP_Death2;
				break;

			case 2:
				SectionName = FName("Death3");
				DeathPose = EDeathPose::EDP_Death3;
				break;

			case 3:
				SectionName = FName("Death4");
				DeathPose = EDeathPose::EDP_Death4;
				break;
		
			default:
				break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}


	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}


	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(EnemyDisappear);//敵が倒されてから消える時間
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();

		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;

			if (HealthBarWidget) 
			{
				HealthBarWidget->SetVisibility(false);
			}
		}

	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}

	if (PlayerComponent && PlayerComponent->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}

	/*パーティクルの大きさと、ランダムで角度を変えて出す演出*/
	FVector Scale(2.5f, 2.5f, 2.5f);
	FRotator RandomRotation
		= FRotator
		(
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f),
			FMath::FRandRange(0.f, 360.f)
		);


	//剣が当たったときの主効果音
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			HitSound,
			ImpactPoint
		);
	}

	//剣が当たったときのサブ効果音
	if (SlashSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			SlashSound,
			ImpactPoint
		);
	}

	//パーティクルを出す
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			HitParticles,
			ImpactPoint,
			RandomRotation,
			Scale
		);
	}
}


void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();

	//敵の位置 Z への低いインパクトポイント
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector   ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	//Forward * ToHit = |Forward||ToHit| * cos(theta)
	//|Forward| = 1 , |ToHit| = 1 , so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//cos(θ)の逆余弦(アーク余弦)を取りθを得る
	double Theta = FMath::Acos(CosTheta);
	//ラジアンから度への変換
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= -45.f && Theta < -135.f)
	{
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);


	/*デバッグ用
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (PlayerComponent && HealthBarWidget)
	{
		PlayerComponent->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(PlayerComponent->GetHealthPercent());//体力を減らす
	}
	CombatTarget = EventInstigator->GetPawn();

	return DamageAmount;
}
