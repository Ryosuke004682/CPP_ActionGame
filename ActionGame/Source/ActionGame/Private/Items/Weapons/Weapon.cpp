// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/PlayerController_Core.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	//武器のアイテムメッシュをPlayerのスケルタルメッシュに取り付ける。
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget , true);
	ItemMesh->AttachToComponent(InParent , TransformRules , InSocketName);


	//動いていた剣を、装備させたら止めるように設定
	Amplitude	  = 0.f;
	TimeConstant  = 0.f;
	RotationSpeed = 0.f;
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex , bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent , OtherActor , OtherComp , OtherBodyIndex);
}
