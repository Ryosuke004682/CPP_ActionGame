#include "Character/PlayerController_Core.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

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
	
}

void APlayerController_Core::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*���̓L�[�֘A*/
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

/*�O���ʏ�ړ�*/
void APlayerController_Core::MoveForward(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;

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

/*���E�ʏ�ړ�*/
void APlayerController_Core::MoveRight(float Value)
{
	if (ActionState == EActionState::EAS_Attacking) return;

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

/*�J��������_���E*/
void APlayerController_Core::Turn(float Value)
{
	AddControllerYawInput(Value);
}

/*�J��������_�㉺*/
void APlayerController_Core::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

/*�A�C�e�����E�����ǂ���*/
void APlayerController_Core::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) 
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));

		//�Ў�ɑ������Ă��锻��ɂ���B
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void APlayerController_Core::Attack()
{

	if(CanAttack())
	{
		PlayerAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}

}


bool APlayerController_Core::CanAttack()
{
	return  ActionState == EActionState::EAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped;
}



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

void APlayerController_Core::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
