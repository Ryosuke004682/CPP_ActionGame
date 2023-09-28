#pragma once

//ECS : �萔�̖ڈ�
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped				UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking  UMETA(DisplayName = "Attacking")
};