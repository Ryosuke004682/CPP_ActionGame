#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterType.h"
#include "PlayerController_Core.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;


UCLASS()
class ACTIONGAME_API APlayerController_Core : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerController_Core();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*Callbacks for input*/
	void MoveForward(float Value);
	void MoveRight	(float Value);
	void Turn		(float Value);
	void LookUp		(float Value);
	void EKeyPressed();
	void Attack();

	/*PlayerMontageFunction*/
	void PlayerAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

private:

	//武器が装備されてない状態の設定
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	EActionState ActionState       = EActionState::EAS_Unoccupied;


	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;


	/*PlayerMontage*/
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* AttackMontage;


public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};