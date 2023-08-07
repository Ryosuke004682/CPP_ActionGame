#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerController_Core.generated.h"

class USpringArmComponent;
class UCameraComponent;


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

private:
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;
};
