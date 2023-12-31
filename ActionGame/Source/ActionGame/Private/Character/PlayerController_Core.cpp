// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerController_Core.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

APlayerController_Core::APlayerController_Core()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch   = false;
	bUseControllerRotationRoll	  = false;
	bUseControllerRotationYaw	  = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f , 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);



	//SpringArmを追加
	Camera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->TargetArmLength = 300.f;

	//カメラを追加
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(Camera);

}

<<<<<<< HEAD
void APlayerController_Core::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("EngageableTarget"));

}

void APlayerController_Core::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
=======
>>>>>>> master

/// <summary>
/// 入力関連をまとめてるところ
/// </summary>
/// <param name="PlayerInputComponent"></param>
void APlayerController_Core::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward") , this , &APlayerController_Core::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight")	, this , &APlayerController_Core::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn")		, this , &APlayerController_Core::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp")		, this , &APlayerController_Core::LookUp);

	//IE_Pressed : 押された場合実行
	PlayerInputComponent->BindAction(FName("Jump")  , IE_Pressed, this , &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip") , IE_Pressed, this , &APlayerController_Core::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this , &APlayerController_Core::Attack);

}

void APlayerController_Core::GetHit_Implementation(const FVector& ImpactPoint)
{
	PlayerHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}


void APlayerController_Core::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("Player"));

}


void APlayerController_Core::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;

}

/// <summary>
/// 攻撃終わりを通知する処理
/// </summary>
void APlayerController_Core::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}




/// <summary>
/// 前後移動の処理
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		// カメラの方向を基準にしたローカル移動方向を計算
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector  Direction       = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FRotator CameraRotation  = ViewCamera->GetComponentRotation();
		const FRotator CameraYawRotation(0.f, CameraRotation.Yaw, 0.f);
		const FVector  CameraDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X);


		AddMovementInput(CameraDirection, Value);
	}
}

/// <summary>
/// 左右移動の処理
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector  Direction	   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		const FRotator CameraRotation  = ViewCamera->GetComponentRotation();
		const FRotator CameraYawRotation(0.f, CameraRotation.Yaw, 0.f);
		const FVector  CameraDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(CameraDirection, Value);
	}
}


/// <summary>
/// カメラ操作_左右の処理
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::Turn(float Value)
{
	AddControllerYawInput(Value);
}

/// <summary>
/// カメラ操作_上下の処理
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


void APlayerController_Core::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		//納刀/抜刀の処理
		if ( CanDisarm() )
		{
			Disarm();
		}
		else if ( CanArm() )
		{
			Arm();
		}
	}

}


/// <summary>
/// 攻撃の処理
/// </summary>
void APlayerController_Core::Attack()
{
	Super::Attack();

	//攻撃可能ならStateをAttackStateに遷移させて攻撃させる。
	if( CanAttack() )
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}


/// <summary>
/// 攻撃可能だったら攻撃のステートの判定を返す処理
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}


/// <summary>
/// 装備してるかどうかの判定を返す処理
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState
					   != ECharacterState::ECS_Unequipped;
}


/// <summary>
/// 手に装備させる
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState 
					   == ECharacterState::ECS_Unequipped && EquippedWeapon;
}


void APlayerController_Core::Disarm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void APlayerController_Core::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}


void APlayerController_Core::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh() , FName("SpineSocket"));
	}
}


void APlayerController_Core::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}


void APlayerController_Core::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName , EquipMontage);
	}
}


void APlayerController_Core::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}