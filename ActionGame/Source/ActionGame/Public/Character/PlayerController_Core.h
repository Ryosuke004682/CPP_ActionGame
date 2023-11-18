#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "PlayerController_Core.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;

UCLASS()
class ACTIONGAME_API APlayerController_Core : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerController_Core();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


protected:
	virtual void BeginPlay() override;

	/*Callbacks for input*/
	void MoveForward(float Value);
	void MoveRight	(float Value);
	void Turn		(float Value);
	void LookUp		(float Value);
	void EKeyPressed();

	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;


	void PlayEquipMontage(const FName& SectionName);
	bool CanDisarm();
	bool CanArm();


	UFUNCTION(BlueprintCallable)
		void Disarm();

	UFUNCTION(BlueprintCallable)
		void Arm();

	UFUNCTION(BlueprintCallable)
		void FinishEquipping();


private:

	//ïêäÌÇ™ëïîıÇ≥ÇÍÇƒÇ»Ç¢èÛë‘ÇÃê›íË
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite , meta = (AllowPrivateAccess = "true"))
	EActionState ActionState       = EActionState::EAS_Unoccupied;


	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* EquipMontage;
};