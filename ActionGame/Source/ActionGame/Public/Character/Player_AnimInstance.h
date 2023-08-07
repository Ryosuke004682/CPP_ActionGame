// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player_AnimInstance.generated.h"

/*
 * 
 */
UCLASS()
class ACTIONGAME_API UPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	//キャラクターを取得
	UPROPERTY(BlueprintReadOnly)
	class APlayerController_Core* PlayerController;

	//移動処理を取得
	UPROPERTY(BlueprintReadOnly , Category = Movement)
	class UCharacterMovementComponent* PlayerMovement;


	UPROPERTY(BlueprintReadOnly , Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly , Category = Movement)
	bool IsJump;

};
