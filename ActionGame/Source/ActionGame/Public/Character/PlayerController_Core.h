#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterType.h"
#include "PlayerController_Core.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;


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

	void MoveForward(float Value);
	void MoveRight	(float Value);
	void Turn		(float Value);
	void LookUp		(float Value);
	void EKeyPressed();

private:

	//ïêäÌÇ™ëïîıÇ≥ÇÍÇƒÇ»Ç¢èÛë‘ÇÃê›íË
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;


	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};