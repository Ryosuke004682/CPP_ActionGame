// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/PlayerController_Core.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	//����̃A�C�e�����b�V����Player�̃X�P���^�����b�V���Ɏ��t����B
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget , true);
	ItemMesh->AttachToComponent(InParent , TransformRules , InSocketName);


	//�����Ă��������A������������~�߂�悤�ɐݒ�
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
