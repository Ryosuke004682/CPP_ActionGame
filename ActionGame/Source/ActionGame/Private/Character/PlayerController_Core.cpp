#include "Character/PlayerController_Core.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

APlayerController_Core::APlayerController_Core()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch   = false;
	bUseControllerRotationRoll	  = false;
	bUseControllerRotationYaw	  = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f , 400.f, 0.f);

	//SpringArm��ǉ�
	Camera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->TargetArmLength = 300.f;

	//�J������ǉ�
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(Camera);

}

void APlayerController_Core::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("Player"));

}

void APlayerController_Core::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// ���͊֘A���܂Ƃ߂Ă�Ƃ���
/// </summary>
/// <param name="PlayerInputComponent"></param>
void APlayerController_Core::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward") , this , &APlayerController_Core::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight")	, this , &APlayerController_Core::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn")		, this , &APlayerController_Core::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp")		, this , &APlayerController_Core::LookUp);

	//IE_Pressed : �����ꂽ�ꍇ���s
	PlayerInputComponent->BindAction(FName("Jump")  , IE_Pressed, this , &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip") , IE_Pressed, this , &APlayerController_Core::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this , &APlayerController_Core::Attack);

}

void APlayerController_Core::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon->IgnoreActors.Empty();
	}
}



/// <summary>
/// �O��ړ��̏���
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		// �J�����̕�������ɂ������[�J���ړ��������v�Z
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
/// ���E�ړ��̏���
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
/// �J��������_���E�̏���
/// </summary>
/// <param name="Value"></param>
void APlayerController_Core::Turn(float Value)
{
	AddControllerYawInput(Value);
}

/// <summary>
/// �J��������_�㉺�̏���
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
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket") , this , this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon  = OverlappingWeapon;
	}
	else
	{
		//�[��/�����̏���
		if ( CanDisarm() )
		{
			PlayEquipMontage(FName("UnEquip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState    = EActionState::EAS_EquippingWeapon;
		}
		else if ( CanArm() )
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState    = EActionState::EAS_EquippingWeapon;
		}
	}

}

/// <summary>
/// �U���̏���
/// </summary>
void APlayerController_Core::Attack()
{
	//�U���\�Ȃ�State��AttackState�ɑJ�ڂ����čU��������B
	if( CanAttack() )
	{
		PlayerAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}


/// <summary>
/// �U���\��������U���̃X�e�[�g�̔����Ԃ�����
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}


/// <summary>
/// �������Ă邩�ǂ����̔����Ԃ�����
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState
					   != ECharacterState::ECS_Unequipped;
}



/// <summary>
/// ��ɑ���������
/// </summary>
/// <returns></returns>
bool APlayerController_Core::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState 
					   == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void APlayerController_Core::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh() , FName("SpineSocket"));
	}
}

void APlayerController_Core::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void APlayerController_Core::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}


/// <summary>
/// �U���p�^�[���������_���Ŏ��s���鏈��
/// </summary>
void APlayerController_Core::PlayerAttackMontage()
{
	 UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)

	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();

		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;

		case 1:
			SectionName = FName("Attack2");
			break;

		case 2:
			SectionName = FName("Attack3");
			break;

		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}



void APlayerController_Core::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquippedMontage)
	{
		AnimInstance->Montage_Play(EquippedMontage);
		AnimInstance->Montage_JumpToSection(SectionName , EquippedMontage);
	}
}


/// <summary>
/// �U���I����ʒm���鏈��
/// </summary>
void APlayerController_Core::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
