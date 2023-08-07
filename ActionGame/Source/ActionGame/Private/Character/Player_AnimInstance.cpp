// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player_AnimInstance.h"
#include "Character/PlayerController_Core.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayer_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//�|�[�����擾����
	PlayerController = Cast<APlayerController_Core>(TryGetPawnOwner());//������
	if (PlayerController)
	{
		PlayerMovement = PlayerController->GetCharacterMovement();
	}
}

void UPlayer_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	//UKismetMathLibrary : ���w�v�Z���Ă����֐�

	if (PlayerMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovement->Velocity);
		IsJump = PlayerMovement->IsFalling();

		PlayerMovement->JumpZVelocity = 250.f;
	}
}
