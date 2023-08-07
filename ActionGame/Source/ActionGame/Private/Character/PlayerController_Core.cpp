#include "Character/PlayerController_Core.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerController_Core::APlayerController_Core()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch   = false;
	bUseControllerRotationRoll	  = false;
	bUseControllerRotationYaw	  = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f , 400.f, 0.f);

	//SpringArmを追加
	Camera = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->TargetArmLength = 300.f;

	//カメラを追加
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

void APlayerController_Core::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward") , this , &APlayerController_Core::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight")	, this , &APlayerController_Core::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn")		, this , &APlayerController_Core::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp")		, this , &APlayerController_Core::LookUp);
}

void APlayerController_Core::MoveForward(float Value)
{
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

void APlayerController_Core::MoveRight(float Value)
{
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

void APlayerController_Core::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerController_Core::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


